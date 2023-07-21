import pandas as pd
from openpyxl import Workbook, load_workbook
from pandas import DataFrame
# 하위 경로 import 방법 3가지
# import source.def_functon as func
# from source import def_functon as func
from source.def_functon import func_name, func_name1, isFuntionName

# from operator import length_hint


# 상위 경로 import 방법
# import sys, os
# sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
# from parent_folder import module_name


# 파일명, 시트명
# fileName = "../Example/test1.xlsx"
# sheetName = ["Sheet1", "Sheet2", "Sheet3"]
fileName = "../Example/SOC_Gauge.xlsx"
sheetName = ["Description", "Privates", "Telltales", "Constants", "Events", "Sounds", "Inters", "Outputs"]
# sheetInfo = []
sheetInfo = list()
excelInfo = dict()

excelInfo["sheetName"] = sheetName  # ["Sheet1", "Sheet2", "Sheet3"]
excelInfo["sheetInfo"] = list()


def excel_parsing(fileName, sheetName, state) :
    # Daraframe형식으로 엑셀 파일 읽기
    # df = pd.read_excel(file_name, header = None, sheet_name=["Sheet1", "Sheet2", "Sheet3"])
    df = pd.read_excel(fileName, sheet_name=sheetName)

    print("\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("\t Parsing Start :", fileName, ", ", sheetName, ", ", len(sheetInfo), ", ", type(sheetInfo))

    index = 0
    if (state) :
        for sheet in sheetName:
            sheetInfo.append(df[sheetName[index]])
            sheetInfo[index] = sheetInfo[index].fillna("")  # 공백 문자(NaN)를 실제 공백으로 변경
            print("\n==================================================================================")
            print(sheetInfo[index])
            index += 1
    else :
        for sheet in excelInfo["sheetName"]:
            excelInfo["sheetInfo"].append(df[excelInfo["sheetName"][index]])
            excelInfo["sheetInfo"][index] = excelInfo["sheetInfo"][index].fillna("")  # 공백 문자(NaN)를 실제 공백으로 변경
            print("\n==================================================================================")
            print(excelInfo["sheetInfo"][index])
            index += 1
    print("\n\t Parsing Complete")
    print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n")





def excel_edit() :
    if (len(sheetInfo[0])) > 0 :
        print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
        print("\t Edit Start")
        print("\n==================================================================================")
        sheetInfo[0].loc[6] =["KKH1", "", "", "KKH4", "KKH5"]
        print(sheetInfo[0])
        print("\n\t Edit Complete")
        print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n")
    else :
        print("Fail to sheetInfo len :", len(excelInfo["sheetInfo"]))




def excel_save(state) :
    if (state) :
        print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
        print("\t Save Start :../Example/sheet1.txt")
        print("\n==================================================================================")
        index = 0
        for sheet in sheetInfo:
            saveSheet = pd.DataFrame(sheet)
            sheetFileName = "../deploy_x86/SFC/" + str(index) + "_" + sheetName[index] + ".txt"
            print("SaveInfo :", sheetFileName, ", ", saveSheet)
            writeSheet = saveSheet.to_csv(sheetFileName, sep="	", index = False)
            index += 1

        # convert_sheet1 = pd.DataFrame(sheetInfo[0])
        # print(convert_sheet1)
        # # write_sheet1 = convert_sheet1.to_csv("../Example/sheet1.txt", sep="	", index = False)
        # write_sheet1 = convert_sheet1.to_csv("../deploy_x86/SFC/sheet1.txt", sep="	", index = False)
        print("==================================================================================")
        print("\n\t Save Complete")
        print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n")
        # test_list = ['one', 'two', 'three']
        # for i in sheet1.loc[0]:
        #     # print("len :", len(i), ", ", i)
        #     print(i)
    else :
        print("")


    index = 0
    with pd.ExcelWriter("../Example/test_convert.xlsx") as w:
        sheetInfo[index].to_excel(w, sheet_name = sheetName[index], index = False)
        index += 1

    # 셀 병합
    # wb = Workbook()
    # ws = wb.active
    # ws.merge_cells("A1:C1")
    # wb.save("../Example/kkh.xlsx")

    wb = load_workbook("../Example/test_convert.xlsx")
    ws = wb[sheetName[0]]
    ws.merge_cells("A2:B3")
    wb.save("../Example/kkh.xlsx")




excel_parsing(fileName, sheetName, True)
# excel_edit()
excel_save(True)


# func.isFuntionName()
# print("FuncName :", func.func_name)

isFuntionName()
print("0 FuncName :", func_name)
print("1 FuncName :", func_name1)

print("excelInfo :", excelInfo, ", len :", len(excelInfo["sheetName"]), ", ", len(excelInfo["sheetInfo"]))
print("sheetName :", excelInfo["sheetName"])
print("sheetInfo :", excelInfo["sheetInfo"])
