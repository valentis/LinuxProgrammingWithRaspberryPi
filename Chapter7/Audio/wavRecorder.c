#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h> 		/* ALSA 라이브러리를 위한 헤더 파일 */
#include "wavFile.h"

WAVHEADER wavheader;

int main(int argc, char **argv)
{
    int fd = -1; 			/* WAV 파일을 위한 파일 디스크립터 */
    int rc, buf_size, dir;
    int format; 			/* 오디오 디바이스 설정을 위한 포맷 */
    long count = 0; 			/* 전체 오디오 데이터의 크기 */
    unsigned int val;
    char *buffer; 			/* 오디오 출력을 위한 데이터 버퍼 */
    snd_pcm_t *handle; 			/* ALSA 디바이스 설정을 위한 구조체 */
    snd_pcm_hw_params_t *params; 	/* 오디오 디바이스 설정을 위한 구조체 */
    snd_pcm_uframes_t frames; 		/* 오디오 프레임 설정을 위한 구조체 */

    if(argc <= 1) { 			/* 저장할 오디오 파일을 명령행 인수로 받는다. */
        printf("usage : %s filename\n", argv[0]);
        return -1;
    }

    /* WAV 파일 열기 */
    if((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
        printf("Could not open the specified wave file : %s＼n", argv[1]);
        return -1;
    }

    /* WAV 헤더 초기화 */
    memset(&wavheader, 0, sizeof(WAVHEADER));

    /* WAV 헤더 설정: 매직넘버 설정 */
    strncpy(wavheader.riffID, "RIFF", 4);
    strncpy(wavheader.waveID, "WAVE", 4);
    strncpy(wavheader.fmtID, "fmt ", 4);
    strncpy(wavheader.dataID,"data",4);

    /* WAV 헤더 설정: 포맷 청크의 설정 */
    wavheader.fmtLen = 16; 		/* FMT 길이 */
    wavheader.fmtTag = 1; 		/* PCM */
    wavheader.nChannels = 1; 		/* 모노(mono) */
    wavheader.nblockAlign = 1; 		/* 8비트 */

    /* 오디오 데이터 속성 설정 */
    wavheader.sampleRate = 8000; 	/* 샘플링 레이트: 8kHz */
    wavheader.bitsPerSample = 8; 	/* 샘플당 비트의 수: 8(byte) */

    /* WAV 파일에 헤더 쓰기 */
    write(fd, &wavheader, sizeof(WAVHEADER));

    /* 녹음을 위한 ALSA PCM 디바이스 열기 */
    rc = snd_pcm_open(&handle, "plughw:1,0", SND_PCM_STREAM_CAPTURE, 0);

    /* 오디오 디바이스에 오디오 디바이스의 매개변수 설정을 위한 공간 확보 */
    snd_pcm_hw_params_alloca(&params);

    /* 기본값으로 초기화 */
    snd_pcm_hw_params_any(handle, params);

    /* 채널 설정하기 */
    snd_pcm_hw_params_set_channels(handle, params, wavheader.nChannels);

    /* 오디오 포맷 설정 */
    format = SND_PCM_FORMAT_U8; 	/* 모노 8비트 */

    /* 인터리브드 모드로 설정 */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* 오디오 디바이스의 포맷 설정 */
    snd_pcm_hw_params_set_format(handle, params, format);

    /* 오디오 디바이스의 샘플링 레이트 설정 */
    val = wavheader.sampleRate;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    /* 32개의 프레임 설정 */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    /* ALSA 드라이버에 오디오 디바이스의 파리미터 적용 */
    rc = snd_pcm_hw_params(handle, params);

    /* 하나의 주기에 필요한 가장 큰 버퍼의 사이즈 얻기 */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    /* 버퍼의 크기 = 프레임의 수 × 채널 × 바이트/샘플 */
    buf_size = frames * wavheader.nChannels * ((format == SND_PCM_FORMAT_S16_LE)?2:1);

    /* 출력을 위한 버퍼 공간 확보 */
    buffer = (char*)malloc(buf_size);

    /* ALSA의 주기 시간을 가져오기 */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);

    /* 사운드 카드로 부터 들어온 데이터를 WAV 파일에 녹화하기 */
    for(int i = 0; i < 500; i++) {
        rc = snd_pcm_readi(handle, buffer, frames);
        if(rc == -EPIPE) {
            /* 오버런(EPIPE)을 경우의 처리 */
            fprintf(stderr, "Overrun occurred＼n");
            snd_pcm_prepare(handle);
        }
        rc = write(fd, buffer, buf_size);
        count += rc;
        if(rc != buf_size) break;
    }

    /* WAV 파일의 크기 변경 */
    lseek(fd, 4, SEEK_SET);
    write(fd, &count, 4);

END:
    close(fd); 			/* 사용이 끝난 파일 닫기 */
    snd_pcm_close(handle); 	/* 사용이 끝난 디바이스 닫기 */

    free(buffer);

    return 0;
}
