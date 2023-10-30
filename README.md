=====================================================================
# [Repo 정보]
	git clone git@github.com.rlarudgh1997:rlarudgh1997/Application.git

# [SFC 정보]
	git clone https://repo.ccos.dev/scm/clu_ivis_private/ccos.core.sfc.git --recursive

# [Docker 정보]
	git clone https://repo.ccos.dev/scm/clu_ivis_private/sfc-docker.git
	# git clone https://repo.ccos.dev/scm/~janginsang_ivis.ai/sfc-docker-jis.git

# [다른 PR fetch 방법 : PR 번호 65 를 ivis.kkh 브랜치로 fetch]
	- 이경우 다른 사람이 요청한 PR 정보의 REPO 접근 권한이 없는 경우에 fetch 하여 사용하는 방법임
	git fetch origin refs/pull-requests/65/from:ivis.kkh
	git fetch origin refs/pull-requests/3367/from:ivis.kkh
	git checkout ivis.kkh

=====================================================================
# [Bitbucket 사용]
- Repo 개인 추가 : bitbucket 내부(https://repo.ccos.dev/projects/CLU_IVIS_PRIVATE/repos/ccos.core.sfc/browse)에서 Create fork 후 생성
	git clone https://repo.ccos.dev/scm/~kkh1997_ivis.ai/ccos.core.sfc.ivis.kkh.git --recursive
	git checkout feature/EXNCP-38725-tc_creator-app
	# git remote add ivis.kkh https://repo.ccos.dev/scm/~kkh1997_ivis.ai/ccos.core.sfc.ivis.kkh.git
	# git checkout ivis.kkh

- Log
	ENH: TC_Creator Application Implement

	Description:
 	- 설명

	Jira: EXNCP-999999999999


- Git Push
	git push ivis.kkh refs/heads/feature/EXNCP-38725-tc_creator-app





 sudo apt-get install qtwebengine5-dev*




=====================================================================
# [SFC Docker 사용법]
- Repo : SFC Docker
	git clone https://repo.ccos.dev/scm/clu_ivis_private/sfc-docker.git

	: 계정 정보 수정 : ~/.git-credentials 참조
		git-credentials
			https://kkh1997%40ivis.ai:Kyeongho2003%21@repo.ccos.dev

	: 도커 설치 가이드
		https://synergy.ccos.dev/display/EXNCP/Alton+Docker

- VMware 에서 사용시
	sudo apt-get update
	docker_build.sh

- 도커 실행 준비
	sudo service docker start
		5ivis!
	sudo chmod -R 777 /var/run/docker.sock

- 도커 이미지 확인 & 삭제
	docker images
	docker rmi -f IMAGE_ID

- 도커 실행
	docker ps
	docker stop 01e1bb164ee8
	~/900_Code/sfc-docker/run_sfc_docker.sh
	docker exec -itu ${USER} -w ${HOME} sfc-latest-${USER} /bin/bash
	docker exec -itu ${USER} -w ${HOME} sfc-latest-${USER} /bin/sh


- 도커 명령어
	: 도커 REPO/TAG 변경 : IMAGE_ID (5d94b61a2ffe)
		docker tag 5d94b61a2ffe artifacts.ccos.dev/sfc-docker/sfc-docker:latest
	: 도커 이미지 삭제 : IMAGE_ID (5d94b61a2ffe), REPO (artifacts.ccos.dev/sfc-docker/sfc-docker)
		docker rmi -f 5d94b61a2ffe
		docker rmi -f artifacts.ccos.dev/sfc-docker/sfc-docker:latest


=====================================================================
# [ToDo 정보]
- 기능 추가
	: NodeAddressSFC.info
	: NodeAddressVSM.info
	: DefaultModule.info
		-> 해당 파일이 생성 될수 있도록 기능 구현

- Open Excel
	: 파일명 Title 에 표시
	: 파일 Open 폴더명과 DeafultModule.info 내부 폴더명과 같으면 현재 Config 저장
		ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, QVariant(selectModule));

- ./model/SFC/CV 하위 폴더 리스트 파일로 생성
	: 생성 파일과 기존(DefaultModule.info) 과 비교하여 생성 리스트로 사용하도록 설정




- 파이썬 코드 수정
	: class 로 변경
	: print 함수를 별도 api 로 변경하여 release, debug 인 경우에 맞도록 프린터 되도록 수정

- Save
	: 저장시 팝업 상태에서 esc 키 동작시 파일 저장됨
	: new 생성후 바로 파일 오픈 하고 편집 하지 않은 상태에서 저장시 저장됨


- 자동완성
	: 편집중 esc 키 입력시 동작 안함
	: 병합셀에서 자동완성 동작시 문제
	: 활성화 상태에서 엔터 입력시 셀에 수정 텍스트 입력 안됨
	: 활성화시 셀에 있는 텍스트 표시 안됨
	: 더블 클릭으로 활성화시 포커스 이동 안됨
	: 입력후 엔터로 완료시 셀 폭 자동 조절 되는 이슈
	: 편집중 다른 화면으로 전환후 new 로 생성하여 f2로 편집시 자동완성 표시 안됨
		: 해당 조건인 경우 포커스를 잃어 버려서 키 입력 이벤트를 받을수 없음


- Open
	: config view 상태에서 open 팝업 표시시 view 화면 사라짐
	: new/open 하여 엑셀 수정 상태에서 다시 open 하는 경우 편집중 저장 팝업 표시 없이 바로 동작함
	: *.fromExcel 파일 정보의 title 로 gui title 구성 하도록
		: 현재는 config title 정보 기반으로 구성 되어 있음
			-> title 정보가 config 와 fromExcel 이 다른 경우 readExcelData 시 문제 발생할수 있음
			-> config title 정보 잘못 입력한 경우 sheet 의 컬럼 정보가 달라서 없는 셀의 정보를 요청 하여 nullptr 오류 발생

- New
	: new 로 생성한 내역이 있는 경우(현재 New 상태) 새로 생성하지 않고 raise or skip 동작 하도록


- SendEvent
	: All 로 전송시 각 컨트롤에서 동작 하도록


- 단축키 기능
	: 추가, 삭제, 병합, 해제 단축키로 동작 하도록

- Edit 기능
	: 복사 & 붙여넣기 기능 추가 - 시트 단위로만 동작

- 전체 일정 작성
	: ~9/15 - 엑셀 편집 기능 추가 수정
	: ~9/22 - 도커 연동 기능(도커 내부에서 gui 표시)


- Top 이름 변경
	: Menu 로 변경



- editConfig 데이터 입력 유효성
	: ConfigSetting 내부에서 조건 처리
		: str 비교 - true, false, 숫자 변경가능한지..



- QTableView 사용방법에 대한 정보
	: https://blog.naver.com/anakt/221834285100

- python lib
	: 프로젝트 빌드시 라이브러리 체크하여 파일 생성
	: 프로그램 실행시 라이브러리 체크 코드는 삭제

- view 메뉴 항목
	: Signal(SFC, Vehicle)
		- VehicleType 별로 해당 하는 항목인지 여부 표시
		- listView & tableView 사용하여 표시
	: Python Lib Install Check


- edit 메뉴 항목
	: 복사 & 붙여넣기


- 엑셀 파일 오픈 & 수정
	: 이상태에서 new 선택시 수정중인 엑셀 파일 저장 팝업 표시 안됨


- 임시 코드 적용
	: Description 내부 병합/해제 되도록 코드 적용됨
		- 해당 기능 필요한지 여부 확인 필요


- 메뉴 팝업 disalbe 처리 조건
	: column 2개 이상 선택인 경우
		- 경고 팝업 : 컬럼을 2개 이상 선택 하였다
	: column 인덱스가 4이상인 상태에서 병합인 경우
		- 경고 팝업 : 병합할수 없는 column 을 선택 하였다.


- 셀 편집시 자동완성 기능
	: _MCAN / _CCAN 으로 된 시그널은 .**__MCAN 글자 전체를 삭제
	: Output 으로 사용한 시그널은 별도 정리 List 로 제공
	: 자동 완성 필요 부분
		[전체 시트 공통]
			Input_Signal : SFC, Vehicle
			Output_Signal : SFC
			Config_Signal : ??
		[Private 시트]
			TC_Name : 다른 부분에 입력되어 있으면 자동완성 기능 동작
		[Outputs 시트]
			.Output_ 으로 시작되는 시그널만 자동완성

- TableView (Copy & Past)
	https://blog.naver.com/PostView.naver?blogId=browniz1004&logNo=221340088463&categoryNo=15&parentCategoryNo=0&viewDate=&currentPage=3&	postListTopCurrentPage=&from=postList&userTopListOpen=true&userTopListCount=30&userTopListManageOpen=false&userTopListCurrentPage=3

	https://blog.naver.com/PostView.naver?blogId=browniz1004&logNo=221340427103&categoryNo=15&parentCategoryNo=0&viewDate=&currentPage=3&postListTopCurrentPage=&from=postList&userTopListOpen=true&userTopListCount=30&userTopListManageOpen=false&userTopListCurrentPage=3


=====================================================================
# [QT 관련 설정]
	- QT5 설치
		sudo apt-get update
		sudo apt-get install make g++ gcc git cmake net-tools
		sudo apt-get install qtchooser qtcreator qt5-default


	- QML 추가 설치 : 아래 순서대로 하나씩 설치해서 이상 유무 확인
		Project ERROR: Unknown module(s) in QT: qml quick multimedia 3drender serialbus serialport

		sudo apt-get install libqt5qml5 libqt5quick5 libqt5serialbus5-dev
		sudo apt-get install qtdeclarative5-dev qml-module-qtquick-controls
		sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5
		sudo apt-get install qml-module-qtmultimedia

		sudo apt-get install libqt5serialport5-dev
		sudo apt-get install qtbase5-dev
		sudo apt-get install qt5*-dev


	- make 오류 : 아래 순서대로 하나씩 설치해서 이상 유무 확인
		sudo apt-get install net-tools
		sudo apt-get install git
		sudo apt-get install lib32stdc++6 build-essential
		sudo apt-get install cmake
		sudo apt-get install lib32z1
		sudo apt-get install libgl1-mesa-dev
		sudo apt-get install gdb


	- QT6 설치
		sudo apt-get update
		sudo apt-get install make g++ gcc git cmake net-tools
		sudo apt-get install qt6-base-dev
		sudo apt-get install qt6-declarative-dev
		sudo apt-get install qml6-module-*

	- qtchooser qmake6 사용하도록 설정 (QT6 사용시)
		cd /usr/bin
		sudo rm qmake
		sudo ln -s /usr/lib/qt6/bin/qmake6 qmake

	- python 설치
		sudo apt-get install python3 python3-pip
		pip install openpyxl pandas
		#sudo ln -s /usr/bin/python3 python
			- Ubuntu_22.04(QT6) : Python 3.10.12
			- Ubuntu_20.04(QT5) : Python 3.8.10

	- Ubuntu 22.04 LTS에 Python 3.8 or 3.9 설치 방법
		https://makebct.net/ubuntu-22-04-lts-%ED%99%98%EA%B2%BD%EC%97%90-python-3-8-%EC%84%A4%EC%B9%98-%EB%B0%A9%EB%B2%95/
		sudo apt-get install software-properties-common
		sudo add-apt-repository ppa:deadsnakes/ppa
		sudo apt-cache policy python3.8
		sudo apt-get install python3.8
		- python 기본 버전 설정
			sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.8 1
			sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.10 2
			sudo update-alternatives --config python
		- python3 기본 버전 설정
			sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1
			sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 2
			sudo update-alternatives --config python3
		python -V
		python3 -V

	- QT Creator 설치
		sudo apt-get install qtcreator
		vi .bashrc : qc alias 경로 수정
			/usr/bin/qtcreator


		- 추가 설치 내역
 			sudo apt-get install libqt6*
			sudo apt-get install libqt6serialport6
			sudo apt-get install qtmultimedia5-dev
			sudo apt-get install libqt6multimedia6 libqt6multimediaquick6 libqt6multimediawidgets6
			sudo apt-get install qml-module-qtmultimedia

	- taglib 오류 발생시 : pkg-config 라이브러리 없어서 발생하는 듯
	sudo apt-get install libtag1-dev
		sudo apt install pkg-config pkgconf
		sudo apt-get install lib32stdc++6 build-essential
		sudo apt-get install lib32z1
		sudo apt-get install libgl1-mesa-dev
		sudo apt-get install gdb

=====================================================================
# [Python 관련 설정]
- lib 설치
	# pip install xlrd
	sudo apt install python python3 python3-pip
	pip install openpyxl pandas
	pip uninstall openpyxl pandas

- lib 설치 확인
	pip list | grep "openpyxl" >> ./CheckLib.txt
	pip list | grep "pandas" >> ./CheckLib.txt
	pip list | grep "xlrd" >> ./CheckLib.txt

- gen_sfc.sh / gen_vsm.sh
	: sfc / vsm 관련 h 파일 생성
	: 생성 경로
cd ~/900_Code/610_Application/SFC/model/ssfs/include/generated
Y:\900_Code\610_Application\SFC\model\ssfs\include\generated


=====================================================================
# [Ubuntu_22.04 + QT6 = Docker]
- qtchooser qmake6 사용하도록 설정 (QT6 사용시)
	cd /usr/bin
	sudo rm qmake
	sudo ln -s /usr/lib/qt6/bin/qmake6 qmake

- PIP 오류 발생시
	sudo apt-get update
	sudo apt-get install python3.8-distutils
	#sudo apt-get install --reinstall python3.8-distutils

	: .bashrc 추가 (현재 사용안함)
		#echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
		#export PATH="$HOME/.local/bin:$PATH"
		#source ~/.bashrc

- 파이썬 엑셀 open 오류 발생시
	[ImportErrrImportError: Unable to import required dependencies:	pytz: No module named 'zoneinfo']
	pip install --upgrade pytz




























=====================================================================
# Repo : 개인 저장소 및 설정 방법
git clone git@github.com.rlarudgh1997:rlarudgh1997/IVIS.git
git clone git@github.com.rlarudgh1997:rlarudgh1997/Personal.git
git clone git@github.com.rlarudgh1997:rlarudgh1997/Tractor.git
git clone git@github.com.rlarudgh1997:rlarudgh1997/Document.git


- 추가 설정 사항
	: 해당 주소 사용시 tractor 와 겹치는 관계로 ./ssh 파일 내용 수정
		# git clone git@github.com:rlarudgh1997/IVIS.git
		./ssh/config
			Host github.com.rlarudgh1997
			Port 22
			HostName github.com
			IdentityFile ~/.ssh/id_rsa_rlarudgh1997
	: 인증키
		./ssh/id_rsa_rlarudgh1997
			- 개인 인증키
		./ssh/id_rsa_rlarudgh1997.pub
			- github 등록된 공용 인증키


- 코드 추가 수정후 commit & push
	git commit --amend
	git push -f


- 파일에서 특정 테스트 위치 찾아서 파일명, 라인 출력
	grep -inr "TEXT"
	grep -inr "TEXT" *







sudo apt-get install qtwebengine5-dev*

sudo apt-get install libgl1-mesa-dev
sudo apt-get install qt6-webengine-dev*
sudo apt-get install libqt6webengine*


#sudo apt-get install qt6-webengine-private-dev*
#sudo apt-get install qt6-webview-dev*
