#############################################################################
#
#   Makefile for post-build step.
#
#   In the Project, the post-build step command line should be
#       nmake /nologo /c /f PostBuild.mak <target>
#
#   In VS2005 you will need to define the predefined macros, eg:
#       nmake /nologo /c /f PostBuild.mak <target> OUTPUT=$(OUTPUT)
#
#   To force a post-build step after every edit of this makefile, specify a
#   custom-build step for this file, and set the output to $(TargetPath).
#   Note that the custom-build command will *not* be executed.
#
#   To force a post-build step even if the target is already successfully
#   build, set the output of the custom-build step to a pseudotarget.
#   Note that the custom-build command *will* be executed.
#
#   Remember: nmake will use the first target if <target> is not specified.
#   Watchout: pseudotarget cannot be name of an existing filesystem object.
#
#############################################################################

CXTOOLS = ..\..\ConnectionTools

DEP1 = ConnectionClient

COPY = @copy /b /y $? $(@D) >nul & @echo created $@

target_default :

target_debug : \
	"$(OUTDIR)\$(DEP1)D.dll"

target_release : \
	"$(OUTDIR)\$(DEP1).dll"

".\Debug\$(DEP1)D.dll" : "$(CXTOOLS)\Debug\$(DEP1)D.dll"
	$(COPY)

".\Release\$(DEP1).dll" : "$(CXTOOLS)\Release\$(DEP1).dll"
	$(COPY)

".\DebugA\$(DEP1)D.dll" : "$(CXTOOLS)\Debug\$(DEP1)D.dll"
	$(COPY)

".\ReleaseA\$(DEP1).dll" : "$(CXTOOLS)\Release\$(DEP1).dll"
	$(COPY)

".\x64\Debug\$(DEP1)D.dll" : "$(CXTOOLS)\x64\Debug\$(DEP1)D.dll"
	$(COPY)

".\x64\Release\$(DEP1).dll" : "$(CXTOOLS)\x64\Release\$(DEP1).dll"
	$(COPY)

".\x64\DebugA\$(DEP1)D.dll" : "$(CXTOOLS)\x64\Debug\$(DEP1)D.dll"
	$(COPY)

".\x64\ReleaseA\$(DEP1).dll" : "$(CXTOOLS)\x64\Release\$(DEP1).dll"
	$(COPY)
