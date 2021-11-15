local function genCode(handler)
    local settings = handler.project:GetSettings("Publish").codeGeneration
    local codePkgName = handler:ToFilename(handler.pkg.name); --convert chinese to pinyin, remove special chars etc.
    local exportCodePath = handler.exportCodePath..'/'..codePkgName
    local namespaceName = codePkgName
    
    if settings.packageName~=nil and settings.packageName~='' then
        namespaceName = settings.packageName..'.'..namespaceName;
    end

    --CollectClasses(stripeMemeber, stripeClass, fguiNamespace)
    local classes = handler:CollectClasses(settings.ignoreNoname, settings.ignoreNoname, nil)
    handler:SetupCodeFolder(exportCodePath, "cs") --check if target folder exists, and delete old files

    local getMemberByName = settings.getMemberByName

    local classCnt = classes.Count
    local writer = CodeWriter.new()
    for i=0,classCnt-1 do
        local classInfo = classes[i]
        local members = classInfo.members
        writer:reset()

        writer:writeln('using FairyGUI;')
        writer:writeln()
        writer:writeln('namespace %s', namespaceName)
        writer:startBlock()
        writer:writeln('public partial class %s : FairyGuiView /* superClass = %s */', classInfo.className, classInfo.superClassName)
        writer:startBlock()

        local memberCnt = members.Count
        for j=0,memberCnt-1 do
            local memberInfo = members[j]
            writer:writeln('public %s %s;', memberInfo.type, memberInfo.varName)
        end
        writer:writeln('public const string URL = "ui://%s%s";', handler.pkg.id, classInfo.resId)
        writer:writeln('public const string Package = "%s";', handler.pkg.name)
        writer:writeln('public const string Component = "%s";', classInfo.resName)
        writer:writeln()
        -- Constructor() 
        writer:writeln('public %s() {} ',classInfo.className)
        writer:writeln()
        -- Constructor(GComponent gc) 
        writer:writeln('public %s(GComponent gc) : base(gc) {}',classInfo.className)
        writer:writeln()

        -- GComponent CreateComponent()
        writer:writeln('public static GComponent CreateComponent()')
        writer:startBlock()
        writer:writeln('return UIPackage.CreateObjectFromURL(URL).asCom;')
        writer:endBlock()
        writer:writeln()

        -- T CreateInstance(GComponet component)
        writer:writeln('public static %s CreateInstance(GComponent component = null)', classInfo.className)
        writer:startBlock()
        writer:writeln('var obj = new %s();',classInfo.className)
        writer:writeln('component ??= UIPackage.CreateObjectFromURL(URL).asCom;',classInfo.className)
        writer:writeln('obj.Bind(component);')
        writer:writeln('return obj;')
        writer:endBlock()
        writer:writeln()

        if handler.project.type==ProjectType.MonoGame then --
            writer:writeln("protected override void OnConstruct()")
            writer:startBlock()
        else -- override to Bind(GComponent component)
            writer:writeln('public override void Bind(GComponent component)')
            writer:startBlock()
            writer:writeln('view = component;')
            writer:writeln()
        end
        for j=0,memberCnt-1 do
            local memberInfo = members[j]
            if memberInfo.group==0 then
                if getMemberByName then
                    if string.sub(memberInfo.type, 1, 3) == "UI_" then 
                        writer:writeln('%s = new %s(component.GetChild("%s").asCom);',memberInfo.varName,memberInfo.type,memberInfo.name)
                    else
                        writer:writeln('%s = (%s)component.GetChild("%s");', memberInfo.varName, memberInfo.type, memberInfo.name)
                    end
                else 
                    writer:writeln('%s = (%s)component.GetChildAt(%s);', memberInfo.varName, memberInfo.type, memberInfo.index)
                end
            elseif memberInfo.group==1 then
                if getMemberByName then
                    writer:writeln('%s = component.GetController("%s");', memberInfo.varName, memberInfo.name)
                else
                    writer:writeln('%s = component.GetControllerAt(%s);', memberInfo.varName, memberInfo.index)
                end
            else
                if getMemberByName then
                    writer:writeln('%s = component.GetTransition("%s");', memberInfo.varName, memberInfo.name)
                else
                    writer:writeln('%s = component.GetTransitionAt(%s);', memberInfo.varName, memberInfo.index)
                end
            end
        end
        writer:endBlock()

        writer:endBlock() --class
        writer:endBlock() --namepsace

        writer:save(exportCodePath..'/'..classInfo.className..'.cs')
    end

    writer:reset()

    local binderName = codePkgName..'/__init__.py'

    writer:writeln('using FairyGUI;')
    writer:writeln()
    writer:writeln('namespace %s', namespaceName)
    writer:startBlock()
    writer:writeln('public class %s', binderName)
    writer:startBlock()

    writer:writeln('public static void BindAll()')
    writer:startBlock()
    for i=0,classCnt-1 do
        local classInfo = classes[i]
        -- writer:writeln('UIObjectFactory.SetPackageItemExtension(%s.URL, () => new %s());', classInfo.className, classInfo.className)
    end
    writer:endBlock() --bindall

    writer:endBlock() --class
    writer:endBlock() --namespace
    
    -- writer:save(exportCodePath..'/'..binderName..'.cs')
end

return genCode