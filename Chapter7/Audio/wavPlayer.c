#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h> 		/* ALSA 라이브러리를 위한 헤더 파일 */
#include "wavFile.h"

WAVHEADER wavheader;

int main(int argc, char **argv)
{
    int fd = -1; 			/* WAV 파일을 위한 파일 디스크립터 */
    int rc, buf_size, dir;
    int channels, format; 		/* 오디오 디바이스 설정을 위한 채널과 포맷 */
    long count, remain;
    unsigned int val;
    char *buffer; 			/* 오디오 출력을 위한 데이터 버퍼 */
    snd_pcm_t *handle; 			/* ALSA 디바이스 설정을 위한 구조체 */
    snd_pcm_hw_params_t *params; 	/* 오디오 디바이스 설정을 위한 구조체 */
    snd_pcm_uframes_t frames; 		/* 오디오 프레임 설정을 위한 구조체 */

    if(argc <= 1) { 			/* 출력할 오디오 파일을 명령행 인수로 받는다. */
        printf("usage : %s filename\n", argv[0]);
        return -1;
    } else {
        printf("Playing file : %s\n", argv[1]);
    }

    /* WAV 파일 열기 */
    if((fd = open(argv[1], O_RDONLY)) == -1) {
        printf("Could not open the specified wave file : %s\n", argv[1]);
        return -1;
    }

    /* WAV 파일로부터 헤더 읽어오기 */
    if((count = read(fd, &wavheader, sizeof(WAVHEADER))) < 1) {
        printf("Could not read wave data\n");
        return -1;
    }

    /* 출력을 위한 ALSA PCM 디바이스 열기 */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        return -1;
    }

    /* 오디오 디바이스에 오디오 디바이스의 매개변수 설정을 위한 공간 확보 */
    snd_pcm_hw_params_alloca(&params);

    /* 기본값으로 초기화 */
    snd_pcm_hw_params_any(handle, params);

    /* 헤더에서 채널에 대한 정보를 가져와서 출력하고 설정하기 */
    channels = wavheader.nChannels;
    printf("Wave Channel Mode : %s\n", (channels-1)? "Stereo":"Mono");
    snd_pcm_hw_params_set_channels(handle, params, channels);

    /* 오디오 포맷 설정 */
    printf("Wave Bytes : %d\n", wavheader.nblockAlign);
    switch(wavheader.nblockAlign) {
        case 1: 			/* 모노 8비트 */
            format = SND_PCM_FORMAT_U8;
            break;
        case 2: 			/* 모노 16비트이거나 스테레오 8비트 */
            format = (channels==1)? SND_PCM_FORMAT_S16_LE : SND_PCM_FORMAT_U8;
            break;
        case 4: 			/* 스테레오 파일인 경우 */
            format = SND_PCM_FORMAT_S16_LE;
            break;
        default:
            printf("Unknown byte rate for sound\n");
            return -1;
    }

    /* 인터리브드 모드로 설정 */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* 오디오 디바이스의 포맷 설정 */
    snd_pcm_hw_params_set_format(handle, params, format);

    /* 오디오 디바이스의 샘플링 레이트 설정 */
    printf("Wave Sampling Rate : 0x%d\n", wavheader.sampleRate);
    val = wavheader.sampleRate;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    /* 출력을 위한 32개의 프레임 설정 */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    /* ALSA 드라이버에 오디오 디바이스의 파리미터 적용 */
    rc = snd_pcm_hw_params(handle, params);
    if(rc < 0) {
        fprintf(stderr, "Unable to set hw parameters: %s\n", snd_strerror(rc));
        goto END;
    }

    /* 하나의 주기에 필요한 가장 큰 버퍼의 사이즈 얻기 */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    /* 버퍼의 크기 = 프레임의 수 × 채널 × 바이트/샘플 */
    //buf_size = frames * wavheader.nblockAlign; //channels * ((format == SND_PCM_FORMAT_S16_LE)?2:1);
    buf_size = frames * channels * ((format == SND_PCM_FORMAT_S16_LE)?2:1);

    /* 출력을 위한 버퍼 공간 확보 */
    buffer = (char*)malloc(buf_size);

    /* ALSA의 주기 시간을 가져오기 */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    remain = wavheader.dataLen; 	/* 패딩을 버리기 위한 초기화 */

    do { 				/* WAV 파일에서 오디오 데이터를 읽어서 ALSA로 출력하기 */
        buf_size = (remain > buf_size)? buf_size: remain; 	/* 패딩을 버리기 위한 계산 */
        if((count = read(fd, buffer, buf_size)) <= 0) break;
        remain -= count;
        rc = snd_pcm_writei(handle, buffer, frames);
        if(rc == -EPIPE) { 		/* 언더런(EPIPE)의 경우일 때 처리 */
            fprintf(stderr, "Underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if(rc < 0) { 		/* 에러가 발생했을 때의 처리 */
            fprintf(stderr, "error from write: %s\n", snd_strerror(rc));
        } else if(rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames\n", rc);
        }
    } while(count == buf_size);

END:
    close(fd);
    sleep(1);

    snd_pcm_drain(handle);
    snd_pcm_close(handle); 		/* 사용이 끝난 디바이스 닫기 */

    free(buffer);

    return 0;
}
