import unreal
import os

# 输出目录
base_output_path = '/FairyGUI-unreal/Class/'

# 设置输出目录，相对于游戏工程
def set_base_output_path(pathname):
    base_output_path = pathname
    return

# 创建或加载蓝图
def load_or_create(fname):
    print('加载文件'+base_output_path+fname)
    bp = unreal.FariyGUIBPCreaterLib.py_create_blueprint('FairyGUIAdapter', base_output_path+fname)
    return bp

# 添加蓝图属性 AddObjAttr(bp,"GImage", "gImage")
def add_obj_attr(bp, varType, varName):
    unreal.FariyGUIBPCreaterLib.py_add_object_member_value(bp, varType, varName)
    return

def add_str_attr(bp, varName, defVal):
    unreal.FariyGUIBPCreaterLib.py_add_string_member_value(bp, varName, defVal)
    return

# 保存蓝图
def compile_save_blue_print(bp):
    unreal.FariyGUIBPCreaterLib.py_compile_save_blue_print(bp)
    return

# 日志输出
def py_log_err(x):
    unreal.FariyGUIBPCreaterLib.py_log_err(x)
    return

# 日志输出
def py_log_trace(x):
    unreal.FariyGUIBPCreaterLib.py_log_trace(x)
    return    