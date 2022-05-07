import os
import shutil
cwd = os.getcwd()
hwzip = os.path.join(cwd, "1102.5281-HW2 - Forward Kinematics-270091.zip")
hwfolder = os.path.join(cwd, "HW2")
reportfolder = os.path.join(hwfolder, "report")
codefolder = os.path.join(hwfolder, "code")
shutil.unpack_archive(hwzip, hwfolder)
try:
    os.mkdir(reportfolder)
    os.mkdir(codefolder)
except FileExistsError:
    pass
for filename in os.listdir(hwfolder):
    currentid = filename.split()[0]
    if filename.endswith("pdf"):
        shutil.move(os.path.join(hwfolder, filename), os.path.join(reportfolder, currentid + ".pdf"))
    elif filename.endswith("cpp"):
        shutil.move(os.path.join(hwfolder, filename), os.path.join(codefolder, currentid + ".cpp"))
