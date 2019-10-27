# LinuxProgrammingWithRaspberryPi
<br>
<h1>Linux Programming With Raspberry Pi(2nd Edition)</h1><br>
출판사 제이펍을 통해 출판한 "사물인터넷을 위한 리눅스 프로그래밍 with 라즈베리 파이(2nd Edition)"의 소스 코드 사이트입니다.<br>
<ul>
  <li>Raspberry pi 4를 기준올 하고 있습니다</li>
  <li>라즈비안(Raspbian)Buster 2019년 9월 26일 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
  <li>8장은 OpenCV 2/3 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
  <li>9장은 라즈베리 파이 4를 기준으로 소스 코드가 작성되어 있고 라즈베리 파이 3 이전에 대한 설명과 소스 코드도 들어 있습니다.</li>
  <li>10장은 Qt 5.11 이후 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
  <li>11장은 Caffe 1.0 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
  <li>12장은 리눅스 커널 5.3.x 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
  <li>13장은 GStreamer 1.0 버전을 기준으로 소스 코드가 작성되어 있습니다.</li>
</ul>
<br>
<h2>1부. 라즈베리 파이 사용하기</h2>
<br>
<h3>1장. 사물인터넷과 라즈베리 파이</h3>
<h4>1.1. 사물인터넷과 오픈 소스 하드웨어 플랫폼</h4>
<h5>&nbsp;&nbsp;1.1.1 제4차 산업혁명과 사물 인터넷</h5>
<h5>&nbsp;&nbsp;1.1.2 오픈 소스의 발전과 오픈 소스 하드웨어의 등장</h5>
<h5>&nbsp;&nbsp;1.1.3 아두이노</h5>
<h4>1.2. 라즈베리 파이 한 입 베어 물기</h4>
<h5>&nbsp;&nbsp;1.2.1 라즈베리 파이</h5>
<h5>&nbsp;&nbsp;1.2.2 라즈베리 파이의 모델 및 구조</h5>
<h4>1.3. 라즈베리 파이를 위한 주변 장치</h4>
<h5>&nbsp;&nbsp;1.3.1 컴퓨터 시스템을 위한 주변 장치</h5>
<h5>&nbsp;&nbsp;1.3.2 라즈베리 파이를 위한 주변 장치</h5>
<h5>&nbsp;&nbsp;1.3.3 라즈베리 파이와 SD 카드</h5>
<h5>&nbsp;&nbsp;1.3.4 라즈베리 파이와 주변 장치</h5>
<h4>1.4 라즈베리 파이를 위한 준비 운동 : 사용 환경 설정</h4>
<h5>&nbsp;&nbsp;1.4.1 라즈베리 파이를 위한 운영체제</h5>
<h5>&nbsp;&nbsp;1.4.2 라즈베리 파이의 디스크 이미지 설치</h5>
<h5>&nbsp;&nbsp;1.4.3 라즈베리 파이의 기본 계정과 부팅 시 환경설정</h5>
<h5>&nbsp;&nbsp;1.4.4 라즈베리 파이의 환경설정</h5>
<h5>&nbsp;&nbsp;1.4.5 네트워크의 사용</h5>
<h4>1.5 요약</h4>
<br>
<h3>2장. 리눅스의 기초: 라즈베리 파이와 친해지기</h3>
<h4>2.1 리눅스의 사용환경과 구조</h4>
<h5>&nbsp;&nbsp;2.1.1 리눅스의 시작</h5>
<h5>&nbsp;&nbsp;2.1.2 리눅스의 특징</h5>
<h5>&nbsp;&nbsp;2.1.3 리눅스의 구조</h5>
<h4>2.2 리눅스의 기본 명령어</h4>
<h5>&nbsp;&nbsp;2.2.1 셸과 프롬프트</h5>
<h5>&nbsp;&nbsp;2.2.2 리눅스의 기본 명령어</h5>
<h5>&nbsp;&nbsp;2.2.3 사용자 계정과 그룹</h5>
<h4>2.3 vi 에디터</h4>
<h5>&nbsp;&nbsp;2.3.1 텍스트 에디터</h5>
<h5>&nbsp;&nbsp;2.3.2 vi 에디터</h5>
<h5>&nbsp;&nbsp;2.3.3 nano 에디터</h5>
<h4>2.4 리눅스의 프로그래밍 도구</h4>
<h5>&nbsp;&nbsp;2.4.1 GCC</h5>
<h5>&nbsp;&nbsp;2.4.2 gcc 컴파일러</h5>
<h5>&nbsp;&nbsp;2.4.3 make 유틸리티</h5>
<h5>&nbsp;&nbsp;2.4.4 gdb 유틸리티</h5>
<h5>&nbsp;&nbsp;2.4.5 라이브러리 만들기</h5>
<h4>2.5 요약</h4>
<br>
<h3>3장. 라즈베리 파이와 GPIO: 하드웨어 제어</h3>
<h4>3.1 GPIO의 개요</h4>
<h5>&nbsp;&nbsp;3.1.1 컴퓨터 구조</h5>
<h5>&nbsp;&nbsp;3.1.2 라즈베리 파이와 GPIO</h5>
<h5>&nbsp;&nbsp;3.1.3 하드웨어의 스케치와 fritzing</h5>
<h4>3.2 LED 제어 프로그래밍</h4>
<h5>&nbsp;&nbsp;3.2.1 전자 부품</h5>
<h5>&nbsp;&nbsp;3.2.2 전자 블록</h5>
<h5>&nbsp;&nbsp;3.2.3 GPIO와 LED</h5>
<h5>&nbsp;&nbsp;3.2.4 echo 명령어와 LED의 점멸</h5>
<h5>&nbsp;&nbsp;3.2.5 gpio 명령어</h5>
<h4>3.3 wiringPi를 이용한 GPIO 프로그래밍</h4>
<h5>&nbsp;&nbsp;3.3.1 wiringPi</h5>
<h5>&nbsp;&nbsp;3.3.2 wiringPi를 이용한 프로그래밍</h5>
<h5>&nbsp;&nbsp;3.3.3 PWM(Pulse Width Modulation)</h5>
<h5>&nbsp;&nbsp;3.3.4 스위치의 사용</h5>
<h5>&nbsp;&nbsp;3.3.5 스피커의 사용</h5>
<h5>&nbsp;&nbsp;3.3.6 조도 센서의 사용</h5>
<h5>&nbsp;&nbsp;3.3.7 DC 모터의 사용</h5>
<h5>&nbsp;&nbsp;3.3.8 7세그먼트의 사용</h5>
<h4>3.4 SenseHAT을 이용한 기상 관측 시스템</h4>
<h5>&nbsp;&nbsp;3.4.1 아스트로 파이 프로젝트와 SenseHAT</h5>
<h5>&nbsp;&nbsp;3.4.2 SenseHAT의 기압 센서</h5>
<h5>&nbsp;&nbsp;3.4.3 SenseHAT의 온습도 센서</h5>
<h5>&nbsp;&nbsp;3.4.4 SenseHAT의 가속도/자이로스코프/지자기 센서</h5>
<h4>3.5 요약</h4>
<br><br>
<h2>2부. 리눅스 기본 프로그래밍</h2>
<br>
<h3>4장. 리눅스 프로그래밍의 기초</h3>
<h4>4.1 리눅스의 기본 구조와 파일 시스템</h4>
<h5>&nbsp;&nbsp;4.1.1 리눅스 시스템의 구조</h5>
<h5>&nbsp;&nbsp;4.1.2 리눅스 파일 시스템</h5> 
<h4>4.2 파일 처리와 표준 입출력</h4>
<h5>&nbsp;&nbsp;4.2.1 저수준 파일 입출력</h5>
<h5>&nbsp;&nbsp;4.2.2 표준 입출력 라이브러리</h5>
<h5>&nbsp;&nbsp;4.2.3 파일 조작 함수</h5>
<h4>4.3 파일 정보와 권한</h4>
<h5>&nbsp;&nbsp;4.3.1 파일 정보와 권한을 위한 리눅스 명령어</h5>
<h5>&nbsp;&nbsp;4.3.2 파일 정보</h5>
<h5>&nbsp;&nbsp;4.3.3 권한과 소유자</h5> 
<h4>4.4 디렉터리와 시간 처리</h4>
<h5>&nbsp;&nbsp;4.4.1 디렉터리</h5>
<h5>&nbsp;&nbsp;4.4.2 유닉스의 시간</h5>
<h4>4.5 리눅스 시스템 프로그래밍과 라즈베리 파이의 제어</h4>
<h4>4.6 요약</h4>
<br>
<h3>5장. 프로세스와 스레드: 다중 처리</h3>
<h4>5.1 프로세스와 시그널</h4>
<h5>&nbsp;&nbsp;5.1.1 프로세스</h5>
<h5>&nbsp;&nbsp;5.1.2 프로세스 관련 명령어</h5> 
<h5>&nbsp;&nbsp;5.1.3 시그널</h5> 
<h4>5.2 멀티 프로세스와 다중 처리 프로그래밍</h4>
<h5>&nbsp;&nbsp;5.2.1 리눅스의 부팅 과정과 프로세스</h5>
<h5>&nbsp;&nbsp;5.2.2 프로세스의 처리</h5> 
<h4>5.3 프로세스 간 통신</h4>
<h5>&nbsp;&nbsp;5.3.1 파이프</h5>
<h5>&nbsp;&nbsp;5.3.2 FIFO</h5>
<h5>&nbsp;&nbsp;5.3.3 유닉스 시스템 V의 IPC 함수</h5> 
<h5>&nbsp;&nbsp;5.3.4 POSIX IPC 함수</h5> 
<h4>5.4 POSIX 스레드와 동기화</h4>
<h5>&nbsp;&nbsp;5.4.1 POSIX 스레드</h5>
<h5>&nbsp;&nbsp;5.4.2 동기화</h5> 
<h4>5.5 다중처리와 라즈베리파이의 제어</h4>
<h5>&nbsp;&nbsp;5.5.1 SenseHAT의 조이스틱(Joystick)</h5>
<h5>&nbsp;&nbsp;5.5.2 스레드를 이용한 병렬 처리</h5> 
<h4>5.6 요약</h4>
<br>
<h3>6장. 리눅스 네트워크 프로그래밍: 사물인터넷의 연결을 위한 기초</h3>
<h4>6.1 네트워크의 개요와 BSD 소켓</h4>
<h5>&nbsp;&nbsp;6.1.1 네트워크의 개요</h5>
<h5>&nbsp;&nbsp;6.1.2 BSD 소켓</h5> 
<h4>6.2 UDP 네트워크 프로그래밍</h4>
<h5>&nbsp;&nbsp;6.2.1 소켓의 사용</h5>
<h5>&nbsp;&nbsp;6.2.2 서버를 위한 bind() 함수</h5> 
<h5>&nbsp;&nbsp;6.2.3 바이트 순서 변환</h5>  
<h5>&nbsp;&nbsp;6.2.4 네트워크 주소 변환</h5> 
<h5>&nbsp;&nbsp;6.2.5 UDP 데이터의 송수신</h5> 
<h5>&nbsp;&nbsp;6.2.6 바이트 조작</h5> 
<h4>6.3 TCP 서버와 클라이언트 프로그래밍</h4>
<h5>&nbsp;&nbsp;6.3.1 TCP 클라이언트</h5>
<h5>&nbsp;&nbsp;6.3.2 TCP 서버</h5>
<h5>&nbsp;&nbsp;6.3.3 소켓의 연결 종료 : shutdown( ) 함수</h5> 
<h5>&nbsp;&nbsp;6.3.4 병행 처리 서버</h5> 
<h4>6.4 HTTP와 웹 서버 프로그래밍</h4>
<h5>&nbsp;&nbsp;6.4.1 웹과 HTTP</h5>
<h5>&nbsp;&nbsp;6.4.2 웹 브라우저와 웹 엔진</h5> 
<h5>&nbsp;&nbsp;6.4.3 HTTP의 구조</h5> 
<h5>&nbsp;&nbsp;6.4.4 HTML</h5> 
<h5>&nbsp;&nbsp;6.4.5 부팅시 웹 서버의 실행</h5> 
<h5>&nbsp;&nbsp;6.4.6 데몬 프로세스</h5> 
<h5>&nbsp;&nbsp;6.4.7 인터넷 패킷 모니터링: Wireshark</h5> 
<h4>6.5 웹 서버와 라즈베리 파이의 제어</h4> 
<h4>6.6 요약</h4>
<br><br>
<h2>3부. 멀티미디어 프로그래밍</h2>
<br>
<h3>7장. 리눅스 멀티미디어 프로그래밍: 현란함과 즐거움</h3>
<h4>7.1 멀티미디어와 라즈베리 파이</h4>
<h5>&nbsp;&nbsp;7.1.1 멀티미디어</h5>
<h5>&nbsp;&nbsp;7.1.2 색상 체계</h5> 
<h5>&nbsp;&nbsp;7.1.3 사운드</h5> 
<h5>&nbsp;&nbsp;7.1.4 아날로그/디지털 변환</h5> 
<h5>&nbsp;&nbsp;7.1.5 리눅스 멀티미디어 시스템</h5>  
<h5>&nbsp;&nbsp;7.1.6 라즈베리 파이의 멀티미디어 시스템</h5> 
<h4>7.2 리눅스 사운드 프로그래밍</h4>
<h5>&nbsp;&nbsp;7.2.1 리눅스 사운드 시스템</h5>
<h5>&nbsp;&nbsp;7.2.2 ALSA를 이용한 오디오 프로그래밍</h5> 
<h5>&nbsp;&nbsp;7.2.3 ALSA 사운드 입출력 프로그래밍</h5>  
<h5>&nbsp;&nbsp;7.2.4 ALSA 믹서 프로그래밍</h5> 
<h5>&nbsp;&nbsp;7.2.5 WAV 파일의 출력</h5>  
<h4>7.3 프레임 버퍼를 통한 이미지 출력</h4>
<h5>&nbsp;&nbsp;7.3.1 프레임 버퍼의 정보 표시</h5>
<h5>&nbsp;&nbsp;7.3.2 프레임 버퍼를 이용한 그래픽</h5>
<h5>&nbsp;&nbsp;7.3.3 SenseHAT의 8×8 LED 매트릭스</h5> 
<h5>&nbsp;&nbsp;7.3.4 BMP 파일 표시하기</h5> 
<h5>&nbsp;&nbsp;7.3.5 Qt를 이용한 이미지 표시</h5> 
<h4>7.4 Video4Linux2와 Pi Camera</h4>
<h5>&nbsp;&nbsp;7.4.1 Video4Linux</h5>
<h5>&nbsp;&nbsp;7.4.2 Pi Camera</h5> 
<h5>&nbsp;&nbsp;7.4.3 Video4Linux2 프로그래밍</h5> 
<h5>&nbsp;&nbsp;7.4.4 BMP 파일로 이미지 캡처</h5> 
<h4>7.5 라즈베리 파이와 멀티미디어</h4> 
<h4>7.6 요약</h4>
<br>
<h3>8장. OpenCV 프로그래밍: 사진 속 얼굴을 찾아라</h3>
<h4>8.1 영상 처리와 OpenCV</h4>
<h5>&nbsp;&nbsp;8.1.1 영상 처리(Image Processing)</h5>
<h5>&nbsp;&nbsp;8.1.2 OpenCV 설치</h5> 
<h4>8.2 OpenCV 기본 프로그래밍</h4>
<h5>&nbsp;&nbsp;8.2.1 Hello World 프로그래밍</h5>
<h5>&nbsp;&nbsp;8.2.2 OpenCV를 이용한 드로잉</h5> 
<h5>&nbsp;&nbsp;8.2.3 마우스 이벤트 핸들러를 이용한 페인터</h5>  
<h5>&nbsp;&nbsp;8.2.4 이미지 파일 저장과 불러오기</h5> 
<h4>8.3 OpenCV와 영상 처리</h4>
<h5>&nbsp;&nbsp;8.3.1 Mat 클래스의 산술 연산</h5>
<h5>&nbsp;&nbsp;8.3.2 OpenCV의 함수와 색상의 변환</h5>
<h5>&nbsp;&nbsp;8.3.3 히스토그램</h5> 
<h5>&nbsp;&nbsp;8.3.4 영상 기하적 변형</h5> 
<h5>&nbsp;&nbsp;8.3.5 이미지 스무싱</h5> 
<h4>8.4 OpenCV를 이용한 얼굴 인식</h4>
<h5>&nbsp;&nbsp;8.4.1 이미지 경계 검사</h5>
<h5>&nbsp;&nbsp;8.4.2 OpenCV와 프레임 버퍼를 이용한 카메라 프로그래밍</h5> 
<h5>&nbsp;&nbsp;8.4.3 OpenCV를 이용한 얼굴 인식</h5> 
<h4>8.5 라즈베리 파이와 OpenCV</h4> 
<h4>8.6 요약</h4>
<br>
<h3>9장. OpenGL ES를 이용한 3D 프로그래밍: 3차원의 세계로…</h3>
<h4>9.1 비디오코어(VideoCore) API와 OpenGL</h4>
<h5>&nbsp;&nbsp;9.1.1 3D 그래픽스와 OpenGL 그리고 OpenGL ES</h5>
<h5>&nbsp;&nbsp;9.1.2 OpenGL ES 1.1과 OpenGL ES 2.0</h5> 
<h5>&nbsp;&nbsp;9.1.3 라즈베리 파이의 멀티미디어 환경</h5> 
<h5>&nbsp;&nbsp;9.1.4 라즈베리 파이의 GPU 초기화</h5> 
<h5>&nbsp;&nbsp;9.1.5 DRM과 GBM</h5> 
<h4>9.2 EGL과 OpenGL ES 1.0</h4>
<h5>&nbsp;&nbsp;9.2.1 EGL(Embedded-System Graphics Library)</h5>
<h5>&nbsp;&nbsp;9.2.2 EGL의 사용과 프로그래밍</h5> 
<h5>&nbsp;&nbsp;9.2.3 OpenGL ES 1.x을 이용한 간단한 프로그래밍</h5>  
<h5>&nbsp;&nbsp;9.2.4 OpenGL의 API 스타일</h5> 
<h5>&nbsp;&nbsp;9.2.5 OpenGL의 좌표계</h5> 
<h5>&nbsp;&nbsp;9.2.6 OpenGL의 모델링과 투영(Projection)</h5> 
<h5>&nbsp;&nbsp;9.2.7 OpenGL의 좌표계 설정</h5> 
<h4>9.3 OpenGL ES 1.1을 이용한 3D 애니메이션</h4>
<h5>&nbsp;&nbsp;9.3.1 OpenGL ES 1.1과 3D 객체 표시</h5>
<h5>&nbsp;&nbsp;9.3.2 OpenGL과 행렬</h5>
<h5>&nbsp;&nbsp;9.3.3 3차원 좌표와 OpenGL ES의 입체 표시</h5> 
<h5>&nbsp;&nbsp;9.3.4 OpenGL 의 변환(Transform)</h5> 
<h5>&nbsp;&nbsp;9.3.5 OpenGL 의 색상</h5> 
<h5>&nbsp;&nbsp;9.3.6 정육면체의 애니메이션</h5> 
<h4>9.4 OpenGL ES 2.0과 셰이딩 언어 (Shading Language)</h4>
<h5>&nbsp;&nbsp;9.4.1 OpenGL의 파이프라인</h5>
<h5>&nbsp;&nbsp;9.4.2 셰이딩 언어</h5> 
<h5>&nbsp;&nbsp;9.4.3 텍스처 매핑</h5> 
<h5>&nbsp;&nbsp;9.4.4 셰이더 언어와 OpenGL ES 2.0</h5> 
<h5>&nbsp;&nbsp;9.4.5 셰이더 언어를 이용한 GPGPU 프로그래밍</h5> 
<h4>9.5 3D와 라즈베리 파이</h4> 
<h4>9.6 요약</h4>
<br><br>
<h2>4부. 리눅스 고급 프로그래밍</h3>
<br>
<h3>10장. Qt를 이용한 GUI 프로그래밍: 더 편리한 접근성</h3>
<h4>10.1 Qt와 라즈베리 파이: Qt on Pi</h4>
<h5>&nbsp;&nbsp;10.1.1 X 윈도 시스템</h5>
<h5>&nbsp;&nbsp;10.1.2 Qt의 개요</h5> 
<h5>&nbsp;&nbsp;10.1.3 라즈베리 파이로 Qt 설치하기</h5> 
<h4>10.2 Qt 프로그래밍과 사용자 정의 위젯</h4>
<h5>&nbsp;&nbsp;10.2.1 Hello World! 프로그래밍</h5>
<h5>&nbsp;&nbsp;10.2.2 Qt의 위젯</h5> 
<h5>&nbsp;&nbsp;10.2.3 Qt에서의 이벤트 처리</h5>  
<h5>&nbsp;&nbsp;10.2.4 사용자 정의 위젯</h5> 
<h5>&nbsp;&nbsp;10.2.5 사용자 정의 시그널과 슬롯</h5> 
<h4>10.3 Qt 위젯과 레이아웃</h4>
<h5>&nbsp;&nbsp;10.3.1 Qt의 기본 위젯</h5>
<h5>&nbsp;&nbsp;10.3.2 버튼 위젯</h5>
<h5>&nbsp;&nbsp;10.3.3 정보 표시 위젯</h5> 
<h5>&nbsp;&nbsp;10.3.4 문자 입력 위젯</h5> 
<h5>&nbsp;&nbsp;10.3.5 범위 선택 위젯</h5> 
<h5>&nbsp;&nbsp;10.3.6 레이아웃</h5> 
<h5>&nbsp;&nbsp;10.3.7 Qt의 다이얼로그</h5> 
<h5>&nbsp;&nbsp;10.3.8 Qt와 Video4Linux2</h5> 
<h4>10.4 Qt Creator를 이용한 GUI 디자인</h4>
<h5>&nbsp;&nbsp;10.4.1 C 언어를 위한 프로젝트 생성과 실행</h5> 
<h5>&nbsp;&nbsp;10.4.2 Qt 애플리케이션을 위한 프로젝트</h5> 
<h4>10.5 GUI와 라즈베리 파이</h4> 
<h4>10.6 요약</h4>
<br>
<h3>11장. 라즈베리 파이와 인공지능: Caffe에서 한잔~</h3>
<h4>11.1 인공지능의 개요</h4>
<h5>&nbsp;&nbsp;11.1.1 인공지능의 등장과 발달</h5>
<h5>&nbsp;&nbsp;11.1.2 머신러닝과 딥러닝</h5> 
<h5>&nbsp;&nbsp;11.1.3 인공지능의 분류</h5>  
<h5>&nbsp;&nbsp;11.1.4 다양한 딥러닝 프레임워크</h5>  
<h4>11.2 Caffe를 이용한 인공지능</h4>
<h5>&nbsp;&nbsp;11.2.1 Caffe의 개요와 설치</h5>
<h5>&nbsp;&nbsp;11.2.2 Caffe를 이용한 이미지의 분류</h5> 
<h5>&nbsp;&nbsp;11.2.3 Caffe의 이미지의 분류 코드</h5>  
<h5>&nbsp;&nbsp;11.2.4 OpenCV를 이용한 카메라와 객체 인식</h5> 
<h4>11.3 Caffe 기본 프로그래밍</h4>
<h5>&nbsp;&nbsp;11.3.1 Caffe의 메인 클래스: 블랍, 레이어, 넷, 해결기</h5>
<h5>&nbsp;&nbsp;11.3.2 Hello Caffe 프로그래밍</h5>
<h5>&nbsp;&nbsp;11.3.3 필러의 사용</h5> 
<h5>&nbsp;&nbsp;11.3.4 블랍</h5> 
<h4>11.4 Caffe를 이용한 숫자 인식</h4>
<h5>&nbsp;&nbsp;11.4.1 숫자 인식을 위한 데이터셋</h5> 
<h5>&nbsp;&nbsp;11.4.2 Qt에서의 필기체 인식을 위한 이벤트 처리</h5> 
<h4>11.5 Caffe와 라즈베리 파이의 제어</h4> 
<h5>&nbsp;&nbsp;11.5.1 OpenCV를 이용한 카메라 인식</h5> 
<h5>&nbsp;&nbsp;11.5.2 Qt에서의 필기체 인식을 위한 이벤트 처리</h5> 
<h4>11.6 요약</h4>
<br>
<h3>12장. 리눅스 커널과 디바이스 드라이버: 더 깊은 곳으로</h3>
<h4>12.1 리눅스 커널과 디바이스 드라이버</h4>
<h5>&nbsp;&nbsp;12.1.1 리눅스 커널과 모듈</h5>
<h5>&nbsp;&nbsp;12.1.2 커널 영역과 사용자 영역</h5> 
<h5>&nbsp;&nbsp;12.1.3 리눅스 디바이스 드라이버</h5>  
<h4>12.2 리눅스 디바이스 드라이버 프로그래밍</h4>
<h5>&nbsp;&nbsp;12.2.1 라즈베리 파이에 리눅스 커널의 설치</h5>
<h5>&nbsp;&nbsp;12.2.2 시스템 호출</h5> 
<h5>&nbsp;&nbsp;12.2.3 커널 모듈</h5>  
<h4>12.3 시스템 레지스터와 LED 출력</h4>
<h5>&nbsp;&nbsp;12.3.1 CPU와 데이터 입출력</h5>
<h5>&nbsp;&nbsp;12.3.2 시스템 레지스터</h5>
<h4>12.4 LED 출력을 위한 GPIO 드라이버 프로그래밍</h4>
<h5>&nbsp;&nbsp;12.4.1 리눅스 디바이스 파일과 GPIO</h5> 
<h5>&nbsp;&nbsp;12.4.2 리눅스 디바이스 드라이버 GPIO 프로그래밍</h5> 
<h4>12.5 스위치를 이용한 커널 이벤트 처리</h4> 
<h5>&nbsp;&nbsp;12.5.1 스위치 사용을 위한 인터럽트 처리</h5> 
<h5>&nbsp;&nbsp;12.5.2 커널에서의 타이머 사용</h5> 
<h5>&nbsp;&nbsp;12.5.3 애플리케이션으로의 시그널 전달</h5> 
<h4>11.6 요약</h4>
<br>
<h2>13. 부록</h2>
<h4>13.1 임베디드 리눅스 공부하기</h4>
<h5>&nbsp;&nbsp;13.1.1 하드웨어</h5> 
<h5>&nbsp;&nbsp;13.1.2 소프트웨어</h5> 
<h4>13.2 리눅스 셸(Shell) 프로그래밍</h4>
<h5>&nbsp;&nbsp;13.2.1 리눅스 셸</h5> 
<h5>&nbsp;&nbsp;13.2.2 bash의 사용</h5> 
<h5>&nbsp;&nbsp;13.2.3 bash의 환경 설정</h5> 
<h5>&nbsp;&nbsp;13.2.4 리눅스 셸 프로그래밍</h5> 
<h4>13.3 GStreamer를 이용한 멀티미디어 프로그래밍</h4>
<h5>&nbsp;&nbsp;13.3.1 GStreamer와 멀티미디어</h5> 
<h5>&nbsp;&nbsp;13.3.2 GStreamer를 이용한 프로그래밍</h5> 
<h5>&nbsp;&nbsp;13.3.3 GStreamer의 도구들</h5> 
<h5>&nbsp;&nbsp;13.3.4 GStreamer와 스트리밍</h5> 
<h4>13.4 ARM NEON 기반의 SIMD 프로그래밍</h4>
<h5>&nbsp;&nbsp;13.4.1 SIMD와 ARM NEON 기술</h5> 
<h5>&nbsp;&nbsp;13.4.2 Hello NEON! 프로그래밍</h5> 
<h5>&nbsp;&nbsp;13.4.3 ARM NEON을 이용한 이미지 처리</h5> 
<h5>&nbsp;&nbsp;13.4.4 ARM NEON 기반의 카메라 이미지 고속 처리</h5>
