#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "8554" 		/* RTSP 서버를 위한 기본 포트 */

static char *port = (char *) DEFAULT_RTSP_PORT;

int main (int argc, char **argv)
{
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;
    GOptionContext *optctx;
    GError *error = NULL;

    /* GOptionContext 객체를 생성한다. */
    optctx = g_option_context_new("RTSP Server\n");
    g_option_context_add_group(optctx, gst_init_get_option_group());
    if(!g_option_context_parse(optctx, &argc, &argv, &error)) {
        g_printerr("Error parsing options: %s\n", error->message);
        return -1;
    }

    g_option_context_free(optctx);

    /* 이벤트 처리를 위한 메인 루프를 생성한다. */
    loop = g_main_loop_new(NULL, FALSE);

    /* 스트리밍 서버를 위한 인스턴스를 생성하고 설정한다. */
    server = gst_rtsp_server_new();
    g_object_set(server, "service", port, NULL);

    /* 서버를 사용하기 위한 기본 마운트 포인터를 획득한다. */
    mounts = gst_rtsp_server_get_mount_points(server);

    /* 스트리밍을 위한 팩토리를 생성한다. */
    factory = gst_rtsp_media_factory_new();

    /* 팩토리를 이용해서 RTSP에 사용할 미디어를 설정한다. */
    gst_rtsp_media_factory_set_launch(factory, "(rpicamsrc bitrate=8500000 "
            "hflip=true vflip=true preview=false ! video/x-h264,width=640,height=480, "
            "framerate=45/1,profile=high ! h264parse ! rtph264pay name=pay0 pt=96 )");

    /* 앞의 팩토리에 /test를 추가한다. */
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

    /* 필요 없는 객체를 해제한다. */
    g_object_unref(mounts);

    /* 기본 메인 콘텍스트에 서버를 붙인다. */
    gst_rtsp_server_attach(server, NULL);

    /* 서버를 시작한다. */
    g_print("stream ready at rtsp://RPI_IP_Address:%s/test\n", port);
    g_main_loop_run(loop);

    /* 필요 없는 객체를 해제한다. */
    g_main_loop_unref(loop);

    return 0;
}
