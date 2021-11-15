local function genCode(handler)
    local settings = handler.project:GetSettings("Publish").codeGeneration
    local codePkgName = handler:ToFilename(handler.pkg.name); --convert chinese to pinyin, remove special chars etc.
    local exportCodePath = handler.exportCodePath..'/'..codePkgName
    local namespaceName = codePkgName
    local ns = 'fgui'
    
    if settings.packageName~=nil and settings.packageName~='' then
        namespaceName = settings.packageName..'.'..namespaceName;
    end

    --CollectClasses(stripeMemeber, stripeClass, fguiNamespace)
    local classes = handler:CollectClasses(settings.ignoreNoname, settings.ignoreNoname, ns)
    handler:SetupCodeFolder(exportCodePath, "ts") --check if target folder exists, and delete old files

    local getMemberByName = settings.getMemberByName

    local classCnt = classes.Count
    local writer = CodeWriter.new({ blockFromNewLine=false, usingTabs=true  })
    for i=0,classCnt-1 do
        local classInfo = classes[i]
        local members = classInfo.members
        local references = classInfo.references
        writer:reset()

        local refCount = references.Count
        if refCount>0 then
            for j=0,refCount-1 do
                local ref = references[j]
                writer:writeln('import %s from "./%s";', ref, ref)
            end
            writer:writeln()
        end

        writer:writeln('import * as fgui from "ue"');
        writer:writeln('export default class %s', classInfo.className)
        writer:startBlock()
        writer:writeln()
        
        local memberCnt = members.Count
        for j=0,memberCnt-1 do
            local memberInfo = members[j]
            if string.sub(memberInfo.type, 1, 15) == 'fgui.Controller' then -- fgui.Controller 改成 fgui.GController
                writer:writeln('public %s:%s;', memberInfo.varName, 'fgui.GController')
            else
                writer:writeln('public %s:%s;', memberInfo.varName, memberInfo.type)
            end
        end

        writer:writeln('public static URL:string = "ui://%s%s";', handler.pkg.id, classInfo.resId)
        writer:writeln('public static Package:string  = "%s";', handler.pkg.name)
        writer:writeln('public static Component:string  = "%s";', classInfo.resName)        
        writer:writeln('public view : fgui.GComponent;')

        writer:writeln()

        -- GComponent CreateComponent()
        writer:writeln('public static  CreateComponent(WorldContextObject: fgui.Object):fgui.GComponent')
        writer:startBlock()
        writer:writeln('return fgui.UIPackage.CreateObjectFromURL(%s.URL, WorldContextObject, fgui.GComponent.StaticClass()) as fgui.GComponent;',classInfo.className)
        writer:endBlock()
        writer:writeln()

        -- T CreateInstance(GComponet component)
        writer:writeln('public static createInstance(component:fgui.GComponent, WorldContextObject: fgui.Object):%s', classInfo.className)
        writer:startBlock()
        writer:writeln('var obj = new %s();',classInfo.className)
        writer:writeln('if(component==null){component = %s.CreateComponent(WorldContextObject);}', classInfo.className)
        writer:writeln('obj.Bind(component, WorldContextObject);')
        writer:writeln('return obj;')
        writer:endBlock()
        writer:writeln()

        -- override to Bind(GComponent component)
        writer:writeln('public  Bind(component:fgui.GComponent, WorldContextObject: fgui.Object):void')
        writer:startBlock()
        writer:writeln('this.view = component;')
        writer:writeln()
        for j=0,memberCnt-1 do
            local memberInfo = members[j]
            if memberInfo.group==0 then
                if getMemberByName then
                    if string.sub(memberInfo.type, 1, 3) == "UI_" then 
                        writer:writeln('this.%s =  %s.createInstance(component.GetChild("%s") as fgui.GComponent, WorldContextObject);',memberInfo.varName,memberInfo.type,memberInfo.name)
                    else
                        writer:writeln('this.%s = <%s>component.GetChild("%s");', memberInfo.varName, memberInfo.type, memberInfo.name)
                    end
                else
                    writer:writeln('this.%s = <%s>(component.GetChildAt(%s));', memberInfo.varName, memberInfo.type, memberInfo.index)
                end
            elseif memberInfo.group==1 then
                if getMemberByName then
                    writer:writeln('this.%s = component.GetController("%s");', memberInfo.varName, memberInfo.name)
                else
                    writer:writeln('this.%s = component.GetControllerAt(%s);', memberInfo.varName, memberInfo.index)
                end
            else
                if getMemberByName then
                    writer:writeln('this.%s = component.GetTransition("%s");', memberInfo.varName, memberInfo.name)
                else
                    writer:writeln('this.%s = component.GetTransitionAt(%s);', memberInfo.varName, memberInfo.index)
                end
            end
        end
        writer:endBlock()

        writer:endBlock() --class

        writer:save(exportCodePath..'/'..classInfo.className..'.ts')
    end

    writer:reset()
end

return genCode