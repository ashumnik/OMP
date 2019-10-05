$env:PATH += "."
Add-Type -Path ".\WebDriver.dll"
$ChromeDriver = New-Object OpenQA.Selenium.Chrome.ChromeDriver 

$URL = "https://miet.ru/schedule"

$ChromeDriver.Navigate().GoToURL($URL)

Start-Sleep -s 2

$GroupField = $ChromeDriver.findElementByXPath("/html/body/main/div[2]/div[2]/div[1]/div[1]/span/span[1]/span")

$GroupField.Click()

$GroupField = $ChromeDriver.findElementByXPath("/html/body/span/span/span[1]/input")
$GroupField.SendKeys("œ»Õ-43")
$GroupField.SendKeys([OpenQA.Selenium.Keys]::Enter)
