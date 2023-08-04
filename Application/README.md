=====================================================================
# [Repo 정보]
git clone git@github.com.rlarudgh1997:rlarudgh1997/Application.git

=====================================================================
# [Python 관련 설정]
- lib 설치
	# pip install xlrd
	pip install openpyxl
	pip install pandas

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
- Open, Save 동작 수정
	: Save 동작시 - Open 한 경우에만 저장 하도록, Open 하지 않았으면 저장 하지 않도록

- 메뉴 항목 추가
	: View
		- Config : 전체 표시, 수정 버튼 활성화
		- Signal(SFC, Vehicle) : VehicleType 별로 해당 하는 항목인지 여부 표시
		- Python Lib Install Check
	: Edit
		- Config : 수정 기능 제공

- parser.py 파일 유무 체크
	: 파일이 없는 경우 default path 폴더에 parser.py 파일 생성(snippet 기능 제공) 및 파일 경로 지정(default path 상대 경로로 지정)
	: read, write 시 파일 존재 체크

- 필요 라이브러리 인스톨 상태 체크 화면 제공 & 인스톨 버튼 제공
	pip install openpyxl pandas
	pip uninstall openpyxl pandas




- 셀 선택후 자동완성 기능 동작확인
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
