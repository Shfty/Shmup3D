 @echo off
 echo Deleting Junk
 echo .
 echo ..
 echo ...
 rmdir /s /q ".\Intermediate\*"
 rmdir /s /q ".\Intermediate\"
 
 rmdir /s /q ".\Saved\Autosaves\"
 rmdir /s /q ".\Saved\Backup\*"
 rmdir /s /q ".\Saved\Collections\*"
 rmdir /s /q ".\Saved\Logs\*"
 rmdir /s /q ".\Saved\Cooked\*"
 
 rm .\*.log
 rm .\*.sln