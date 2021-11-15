import os
import sys
import unreal
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import common
import pub_fairy_gui


common.py_log_trace(" \n\n=======================================exec start===================================\n\n")

# # "目录的删除维护通过脚本来支持，运行时处理引用关系会出问题。发布时先删除所有目录，再生成资源"
# bp = common.load_or_create('Gen005')

# common.add_str_attr(bp, "URL", "ui://sn18zihk8w3v0")
# common.add_str_attr(bp, "Package", "12")
# common.add_str_attr(bp, "Component", "123")
# common.add_obj_attr(bp,"GImage", "myImage")
# common.compile_save_blue_print(bp)

# " 创建所有蓝图 "
# " 给蓝图添加变量 "
# " 给蓝图添加函数 "
# " 保存 "
common.py_log_trace(" \n\n========================================exec end==================================！！！\n\n")

# py "E:\FUI\utest\FairyGUI-unreal-example\Content\Python\main.py"
# Blueprint'/Game/FairyGUI/Gen005.Gen005'
# init_unreal.py InAssetPath = 0x0000019230b8b980 L"/Game/FairyGUI/MainPage"