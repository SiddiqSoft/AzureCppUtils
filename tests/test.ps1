[Reflection.Assembly]::LoadWithPartialName("System.Web")| out-null
$URI="https://myNamespace.servicebus.windows.net/myEventHub"
$Access_Policy_Name="RootManageSharedAccessKey"
$Access_Policy_Key="myPrimaryKey"
#Token expires now+300
$NowT=([DateTimeOffset]::Now.ToUnixTimeSeconds())
$Expires=$NowT+300
Write-Host "Now: $NowT --> $Expires"
$SignatureString=[System.Web.HttpUtility]::UrlEncode($URI)+ "`n" + [string]$Expires
Write-Host "SignatureString: $SignatureString"
$HMAC = New-Object System.Security.Cryptography.HMACSHA256
$HMAC.key = [Text.Encoding]::ASCII.GetBytes($Access_Policy_Key)
$Signature = $HMAC.ComputeHash([Text.Encoding]::ASCII.GetBytes($SignatureString))
$Signature = [Convert]::ToBase64String($Signature)
$SASToken = "SharedAccessSignature sr=" + [System.Web.HttpUtility]::UrlEncode($URI) + "&sig=" + [System.Web.HttpUtility]::UrlEncode($Signature) + "&se=" + $Expires + "&skn=" + $Access_Policy_Name
Write-Host "SASToken: $SASToken"