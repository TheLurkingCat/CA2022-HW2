echo "ID, punch_FK, walk_FK, run_FK, 160/150, 175/135, 150/160" >out.csv
Get-ChildItem -Path ../bin/ -Filter *.exe | ForEach {
	[System.IO.File]::AppendAllText("out.csv", "$([System.IO.Path]::GetFileNameWithoutExtension($_))")
    &$_.Fullname 1>$null 2>>out.csv
    if ( $? -ne $true) {
		[System.IO.File]::AppendAllText("out.csv", ", R")
    }
	
}