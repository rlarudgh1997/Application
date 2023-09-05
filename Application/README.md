=====================================================================
# [Repo 정보]
git clone git@github.com.rlarudgh1997:rlarudgh1997/Application.git

=====================================================================
# [Python 관련 설정]
- lib 설치
	# pip install xlrd
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
# ToDo
- 데모 리뷰 순서
	: default path 기능 설명
	: node address path 기능 설명
	: 기존 파일 open 기능 설명
	: new 기능 설명
	: save & save as 기능 설명
	: merge 기능 설명
	: split 기능 설명
	: insert 기능 설명
	: delete 기능 설명
	: config 표시, 수정 기능 설명
	: node addres 표시 기능 설명
	: node addres 자동완성 기능 설명



- editConfig 데이터 입력 유효성

- initCommonData 이동
- initNormalData 이동




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
