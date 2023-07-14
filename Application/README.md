=====================================================================
# [Repo 정보]

git clone git@github.com.rlarudgh1997:rlarudgh1997/Application.git


=====================================================================
# [Python 관련 설정]

- lib 설치
	pip install xlrd
	pip install openpyxl
	pip install pandas

- lib 설치 확인
	pip list | grep "openpyxl" >> ~/check_python_lib_state.txt

- gen_sfc.sh / gen_vsm.sh
	: sfc / vsm 관련 h 파일 생성
	: 생성 경로
cd ~/900_Code/610_Application/SFC/model/ssfs/include/generated
Y:\900_Code\610_Application\SFC\model\ssfs\include\generated


=====================================================================
# [프로젝트 진행 사항]

- New 선택시 : count 만큼 sheet 생성
- 셀 선택후 자동완성 기능 동작확인

- TableView (Copy & Past)
	https://blog.naver.com/PostView.naver?blogId=browniz1004&logNo=221340088463&categoryNo=15&parentCategoryNo=0&viewDate=&currentPage=3&	postListTopCurrentPage=&from=postList&userTopListOpen=true&userTopListCount=30&userTopListManageOpen=false&userTopListCurrentPage=3

	https://blog.naver.com/PostView.naver?blogId=browniz1004&logNo=221340427103&categoryNo=15&parentCategoryNo=0&viewDate=&currentPage=3&postListTopCurrentPage=&from=postList&userTopListOpen=true&userTopListCount=30&userTopListManageOpen=false&userTopListCurrentPage=3


- SFC 테스트 파일 로딩 하여 정부 수정 및 테스트 진행
	: 엑셀 파일 로딩 (파이썬 이용)
		: 위치, 파일 앱에서 파이썬으로 전달하여 엑셀 파싱 및 결과 리턴
	: 파이썬
		: excel to csv 변환
		: csv to excel 변환

- .UI 파일을 사용하여 GUI 화면 구성
	: 기존 코드 형태 사용
	: UI 내용 구성을 코드로 작성
		: class - sub1 class - sub2 class
		GuiMainHander.cpp
			DataModel.h
				: int currentDepth
				: int currentMode
				: int currentScreen
			DataModelMain.h
		ControlMain.cpp


=====================================================================
# ToDo

- Applicatoin
	: 소켓 서버 구현
		: python(daemon 형태) <-> app 간 프로토콜 정의 하여 상호 연동
	: 특정 프로그램의 로그 출력을 실시간으로 화면 표출 가능성 검토
		: tail 기능 사용

- QML vs UI 파일 구현 비교
	: QML - ApplicationWindow 사용방법 -> main 화면만 해당 qml 로 구성, 다른 화면은 Item 으로 구성

- Web App 으로 구현 방법 검토
	: client - javascript, php 등으로 화면 구성 (GUI 화면 구성)
	: server - 아파치 등 (메인기능에 대한 API 필요)
		: 로컬서버 사용 가능성 검토
		: 다수의 사람이 동시 작업이 필요한 경우 서버 구성 필요
	: 이슈
		: 해당 언어에 대하 지식 부족 -> 구현 가능성은 높으나 시간이 걸릴듯
		: 특정 요구사항에 대한 대응이 가능한지 검토 필요
			: 엑셀 로딩, 편집, 저장 등의 기능
			: sfc test 스크립트와의 연동 기능
			: 스크립트 연동


- Kick-off 회의에서 T/C Creator 관련하여 논의한 내용 공유
	: T/C Creator 에서 엑셀의 셀 편집 기능(병합...)과 내용 입력시 자동완성기능이 중요하다.
		: 기본적인 셀 병합/분할 기능 가능
		: 자동완성 기능 가능 - 특정 알파벳 입력시 결과를 리스트로 제공
	: 엑셀을 로드/수정/저장 하는데 문제점 및 기술적 난이도에 대해서 정확하게 알려 달라.
		: 엑셀 로드 - python 을 사용하여 엑셀 파싱 -> sheet 별 파일로 저장 -> 해당 파일 App 에서 로딩
		: 엑셀 수정 - 기본 기능(텍스트 수정, 셀 병합/분할)에 대한 기능 QTableWidget 클래스 사용하여 구현 가능
		: 엑셀 저장 - QTableWidget 저장된 데이터 -> sheet 별 별도 파일로 저장 -> python 사용하여 엑셀로 변환
		: 예상 이슈 사항
			- python 사용하여 엑셀 저장시 병합된 셀이 정확히 표시가 가능하지 여부
	: Application 또는 Web-browser (stand alone)를 사용한 개발에 대해 검토해 달라.
		: Application 개발
			- QT .ui 를 활용하여 개발 및 초기 빠른 개발 가능
		: Web-browser 개발
			- client(javascript/php 등), server(Apache 등) 을 사용하여 개발
			- 필요한 개발 언어에 대한 경험 부족으로 초기 개발 진척이 느릴것으로 예상됨
	: 상세 기능은 협의 후 진행 예정.
