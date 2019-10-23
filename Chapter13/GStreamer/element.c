#include <gst/gst.h>

int main(int argc, char **argv)
{
    GstElement *pipeline, *source, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* GStreamer 초기화 */
    gst_init(&argc, &argv);

    /* 엘리먼트의 생성 */
    source = gst_element_factory_make("videotestsrc", "source");
    sink = gst_element_factory_make("autovideosink", "sink");

    /* 빈 파이프라인 생성 */
    pipeline = gst_pipeline_new("test-pipeline");
    if(!pipeline || !source || !sink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* 파이프라인의 구성 */
    gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);
    if(gst_element_link(source, sink) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* 소스 엘리먼트의 설정 */
    g_object_set(source, "pattern", 0, NULL);

    /* 재생 상태로 변경 */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* 재생이 끝날 때까지 대기 */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
    GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* GStreamer의 사용 후 자원의 해제 */
    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
