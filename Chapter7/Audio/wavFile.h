#ifndef __WAVHEADER_H
#define __WAVHEADER_H

typedef struct wavFile_t {
    unsigned char riffID[4]; 		/* "RIFF" : riffID[ ] = {'R', 'I', 'F', 'F'}; */
    unsigned long riffLen; 		/* 다음에 나오는 데이터의 크기 */
    unsigned char waveID[4]; 		/* "WAVE" : waveID[ ] = {'W', 'A', 'V', 'E'}; */
    unsigned char fmtID[4]; 		/* "fmt " : fmtID[ ] = {'f', 'm', 't', ' '}; */
    unsigned long fmtLen; 		/* 일반적으로 16 */
    unsigned short fmtTag; 		/* 일반적으로 PCM(1) */
    unsigned short nChannels; 		/* 채널의 수 */
    unsigned long sampleRate; 		/* 샘플링 레이트 */
    unsigned long avgBytesPerSec; 	/* 초당 바이트 수(Bytes / sec) */
    unsigned short nblockAlign; 	/* 한 샘플당 바이트 수 : 일반적으로 1 */
    unsigned short bitsPerSample; 	/* 샘플당 비트 수(Bits / sample) */
    unsigned char dataID[4]; 		/* "data" : dataID[ ] = {'d', 'a', 't', 'a'}; */
    unsigned long dataLen; 		/* 데이터의 크기 */
} WAVHEADER;

#endif
