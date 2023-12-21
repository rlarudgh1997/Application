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
	git fetch origin refs/pull-requests/3376/from:ivis.kkh
	git checkout ivis.kkh

# [VSM 모듈 pull 안될경우]
	cd ccos.core.sfc/model
	rm -rf VSM
	git clone https://repo.ccos.dev/scm/clu_ivis_private/ccos.core.vsm.git
	mv ccos.core.vsm VSM

# [Repo 새로 받지 않고 사용하는 방법]
	sudo rm -rf *
	git checkout -f
	git submodule init
	git submodule update


=====================================================================
# [Bitbucket 사용]
- Repo 개인 추가 : bitbucket 내부(https://repo.ccos.dev/projects/CLU_IVIS_PRIVATE/repos/ccos.core.sfc/browse)에서 Create fork 후 생성
	git clone https://repo.ccos.dev/scm/~kkh1997_ivis.ai/ccos.core.sfc.ivis.kkh.git --recursive
	git checkout feature/EXNCP-38725-tc_creator-app
	# git remote add ivis.kkh https://repo.ccos.dev/scm/~kkh1997_ivis.ai/ccos.core.sfc.ivis.kkh.git
	# git fetch ivis.kkh
	# git checkout [branch name]
	## 에러 발생시 :  modified: model/VSM (new commits)
	# git submodule update


	https://repo.ccos.dev/projects/CLU_IVIS_PRIVATE/repos/ccos.core.sfc/pull-requests/3524/overview


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
		https://haengsin.tistory.com/128

- VMware 에서 사용시
	sudo apt-get update
	docker_build.sh

- 도커 실행 준비
	sudo service docker status
	sudo service docker start
		5ivis!
	sudo chmod -R 777 /var/run/docker.sock

- 도커 이미지 확인 & 삭제
	docker images
	docker rmi -f IMAGE_ID

- 도커 실행
	docker ps
	docker stop sfc-latest-ivis
	~/900_Code/sfc-docker/run_sfc_docker.sh
	docker exec -itu ${USER} -w ${HOME} sfc-latest-${USER} /bin/bash
	docker exec -itu ${USER} -w ${HOME} sfc-latest-${USER} /bin/sh

	~/900_Code/sfc-docker/WSL_run_sfc_docker.sh
	docker exec -itu ${USER} -w ${HOME} sfc-latest-${USER} /bin/bash


- 도커 명령어
	: 도커 REPO/TAG 변경 : IMAGE_ID (5d94b61a2ffe)
		docker tag 5d94b61a2ffe artifacts.ccos.dev/sfc-docker/sfc-docker:latest
	: 도커 이미지 삭제 : IMAGE_ID (5d94b61a2ffe), REPO (artifacts.ccos.dev/sfc-docker/sfc-docker)
		docker rmi -f 5d94b61a2ffe
		docker rmi -f artifacts.ccos.dev/sfc-docker/sfc-docker:latest



=====================================================================
# [도커 내부에서 검증 동작 수행시]
- Alton 환경 변수
	export ALTON_SFC_CONFIGURATION=${HOME}/900_Code/620_SFC/model/SFC/compiled/config
	export ALTON_SFC_MODEL_DIR=${HOME}/900_Code/620_SFC//model/SFC/compiled
	export ALTON_EOL_VEHICLETYPE=ICV
	export ALTON_SFC_CONFIGURATION_FILENAME=CV
	export ALTON_ENG_SKIP_DUMMYCALL=ON

- SFC 빌드 명령어
	cd ${HOME}/900_Code/620_SFC
	mkdir build
	cd build
	# PV
		cmake .. -DCCOS_LIB_VERSION=2.1.0 -DSFC_BUILD_TYPE=coverage
		sudo cmake --build . --target install -j8
	# CV
		cmake .. -DCCOS_LIB_VERSION=2.1.0 -DVEHICLE_CATEGORY=CV -DSFC_BUILD_TYPE=coverage
		sudo cmake --build . --target install -j8

- Validator 빌드
	cd ${HOME}/900_Code/620_SFC/validator
	mkdir build
	cd build
	cmake ..
	make -j8

- GenTC, RunTC
	./gen_tc.sh -c CV -m "ABS_CV AVH BCM_Warning_CV BEQ Brake_Air"
	./run_tc.sh -b /usr/local/bin/altonservice -c CV -d -g -m "BCM_Warning_CV VCU Transmission_Warning_CV"

- Run TC 수행 모듈
	BCM_Warning_CV
	VCU
	Transmission_Warning_CV



=====================================================================
# [VNC 정보]
	vncserver -list
	vncserver -kill :1
	vncserver -localhost no -geometry 1400x900
	# vncserver -localhost no -geometry 1400x900 -SecurityTypes None


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
# Clang-format 설정

[clang 우분투 설치]
	wget https://apt.llvm.org/llvm.sh
	chmod +x llvm.sh
	sudo ./llvm.sh 13
	sudo apt-get install clang-format-13


[clang 윈도우 설치]
	https://blog.naver.com/websearch/222344931042


[bin 위치 : vscode 설정]
	clang-foramt 위치 설정
		/usr/bin/clang-format-13
		C:\Program Files\LLVM\bin\clang-format.exe


[clang 체크]
	./sfc_clang_format.sh -a





=====================================================================
# Test Application 설정

[참조]
	https://synergy.ccos.dev/pages/viewpage.action?pageId=79941712

[SDK 설치]
	설치 경로 : ~/usr
	git clone https://repo.ccos.dev/scm/clu_ivis_private/ccos.app.sfc-testapp.git


[protobuf 설치]
	설치 경로 : 아무곳이나
	wget https://github.com/protocolbuffers/protobuf/releases/download/v3.13.0/protobuf-all-3.13.0.tar.gz
	tar -zxvf protobuf-all-3.13.0.tar.gz
	mv protobuf-3.13.0 protobuf
	cd protobuf
	sudo apt-get install autoconf automake libtool curl make unzip libssl-dev
	./autogen.sh
	./configure
	make; make check; sudo make install
	sudo ldconfig


[SFC 빌드]
	ssfs 이동 복사 : repo 빌드시에 하나로 통합해서 나옴(확인 필요)
		lib	: ~/usr/ccos.app.sfc-testapp/host/lib
			620_SFC/build/model/ssfs
		include 복사 : ~/usr/ccos.app.sfc-testapp/host/include/ssfs
			620_SFC/model/ssfs/include/external
			620_SFC/model/ssfs/include/generated
			620_SFC/model/ssfs/include/public








=====================================================================
# Repo : 개인 저장소 및 설정 방법
git clone git@github.com.rlarudgh1997:rlarudgh1997/Application.git
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
