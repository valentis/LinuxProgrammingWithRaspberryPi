#include <stdio.h>
#include <alsa/asoundlib.h>

int main(int argc, char **argv)
{
    snd_mixer_t *mixer;
    snd_mixer_elem_t *elem; 		/* 믹서를 위한 PCM 엘리먼트(element) */
    snd_mixer_selem_id_t *id;
    int status;
    long maxVal = 100, minVal = 0, outVal;

    if(argc <= 1) { 			/* 설정할 볼륨의 크기를 명령행 인수로 받는다. */
        printf("usage : %s volume\n", argv[0]);
        return -1;
    }

    outVal = atoi(argv[1]);
    static const char* mix_name = "PCM"; 	/* 이전 버전은 'PCM'을 사용 */
    static const char* card = "default";
    static int mix_index = 0;

    snd_mixer_open(&mixer, 0); 		/* mixer 디바이스 열기 */
    snd_mixer_attach(mixer, card); 	/* 기본 믹서 사용하기 */
    snd_mixer_selem_register(mixer, NULL, NULL);
    snd_mixer_load(mixer); 		/* 믹서 불러오기 */

    /* 믹서 사용을 위한 ID 객체의 할당 */
    snd_mixer_selem_id_alloca(&id); 	/* snd_mixer_selem_id_malloc() */

    /* ID에 믹서의 이름(name)과 인덱스(Index) 설정 */
    snd_mixer_selem_id_set_index(id, mix_index);
    snd_mixer_selem_id_set_name(id, mix_name);

    /* 설정된 ID를 이용해서 해당 믹서의 엘리먼트 찾기 */
    elem = snd_mixer_find_selem(mixer, id);

    /* 볼륨의 범위(최솟값/최댓값)를 설정하고 현재의 값을 설정하기 */
    outVal = (outVal * (maxVal - minVal) / (long)(100-1)) + minVal;
    snd_mixer_selem_set_playback_volume_range(elem, minVal, maxVal);
    snd_mixer_selem_set_playback_volume_all(elem, outVal);

    /* 현재 설정된 믹서의 범위 가져와서 화면에 표시 */
    snd_mixer_selem_get_playback_volume_range(elem, &minVal, &maxVal);
    fprintf(stderr, "Set volume %i(%d/%d)\n", outVal, maxVal, minVal);

    snd_mixer_close(mixer); 		/* 믹서(mixer) 닫기 */

    return 0;
}
