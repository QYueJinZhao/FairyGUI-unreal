local function genCode(handler)
    local settings = handler.project:GetSettings("Publish").codeGeneration
    local codePkgName = handler:ToFilename(handler.pkg.name); --convert chinese to pinyin, remove special chars etc.
    local exportCodePath = handler.exportCodePath..'/'..codePkgName
    local namespaceName = codePkgName
    local ns = 'fgui'
    local cache_lines = {}
    if settings.packageName~=nil and settings.packageName~='' then
        namespaceName = settings.packageName..'.'..namespaceName;
    end

    local classes = handler:CollectClasses(settings.ignoreNoname, settings.ignoreNoname, ns)
    handler:SetupCodeFolder(exportCodePath, "py") --check if target folder exists, and delete old files

    local getMemberByName = settings.getMemberByName

    local classCnt = classes.Count
    local writer = CodeWriter.new({ blockFromNewLine=false, usingTabs=true  })
    for i=0,classCnt-1 do
        local classInfo = classes[i]
        local members = classInfo.members
        local references = classInfo.references

        writer:reset()

        writer:writeln('#!/usr/bin/python')
        writer:writeln('# -*- coding: UTF-8 -*-')
        writer:writeln('import common')

        writer:writeln()
        -- writer:writeln('common.py_log_trace(" =======================================exec start %s===================================")',classInfo.className)

        writer:writeln('bp = common.load_or_create("%s")', codePkgName..'/'..classInfo.className)        
        writer:writeln('common.add_str_attr(bp, "URL", "ui://%s%s")',handler.pkg.id, classInfo.resId)
        writer:writeln('common.add_str_attr(bp, "Package", "%s")',handler.pkg.name)
        writer:writeln('common.add_str_attr(bp, "Component", "%s")',classInfo.resName)


        writer:writeln()
     
        local memberCnt = members.Count
        for j=0,memberCnt-1 do
            local memberInfo = members[j]
     
            if string.sub(memberInfo.type, 1, 5) == 'fgui.' then
                if string.sub(memberInfo.type, 1, 15) == 'fgui.Controller' then -- fgui.Controller 改成 fgui.GController
                    writer:writeln('common.add_obj_attr(bp, "GController", "%s")',memberInfo.varName)
                else
                    writer:writeln('common.add_obj_attr(bp, "%s", "%s")',string.sub(memberInfo.type, 6),memberInfo.varName)
                end
            else
                writer:writeln('common.add_obj_attr(bp, "%s", "%s")',memberInfo.type, memberInfo.varName)                
            end
        end
     

        writer:writeln()

        writer:writeln('common.compile_save_blue_print(bp)')
        -- writer:writeln('common.py_log_trace(" =======================================exec end %s===================================")',classInfo.className)
        writer:save(exportCodePath..'/'..classInfo.className..'.py')
    end
    writer:reset()
end

return genCode