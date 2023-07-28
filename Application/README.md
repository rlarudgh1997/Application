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
# ToDo
- 메뉴 항목 추가
	: View
		- Config : 전체 표시, 수정 버튼 활성화
		- Signal(SFC, Vehicle) : VehicleType 별로 해당 하는 항목인지 여부 표시
		- Python Lib Install Check
	: Edit
		- Config : 수정 기능 제공
	: Exit
		- 수정중인 경우 팝업 표시

- parser.py 파일
	: read, write 시 파일 존재 체크
	: snippet 기능 제공

- updateDataHandler(PropertyTypeEnum::PropertyTypeSaveFilePath, filePath);
	: 함수 동작 수정
	: new - edit - exit - ok --> save as
	: open - edit- exit - ok --> save
	




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
