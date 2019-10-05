$env:PATH += "."
Add-Type -Path ".\WebDriver.dll"
Add-Type -Path ".\WebDriver.Support.dll"
$ChromeDriver = New-Object OpenQA.Selenium.Chrome.ChromeDriver 

$URL = "https://miet.ru/schedule"

$ChromeDriver.Navigate().GoToURL($URL)

Start-Sleep -s 1

$GroupField = $ChromeDriver.findElementByXPath("/html/body/main/div[2]/div[2]/div[1]/div[1]/span/span[1]/span")

$GroupField.Click()

$GroupField = $ChromeDriver.findElementByXPath("/html/body/span/span/span[1]/input")
$GroupField.SendKeys("ПИН-43")
$GroupField.SendKeys([OpenQA.Selenium.Keys]::Enter)


$Today = $ChromeDriver.findElementByXPath("/html/body/main/div[2]/div[2]/div[1]/div[2]")
Start-Sleep -s 1
$Today.Click()

$Subject = $ChromeDriver.findElementByXPath("/html/body/main/div[2]/div[2]/div[2]/table/tbody/tr[5]/td/div/div/div[1]")

$WhereAndWhat = $Subject.Text

cls

if($WhereAndWhat -eq "4212 а | Теория автоматов и формальных языков [Лаб]"){    
    echo "Test passed"
}
else{
    echo "Test failed"
}

