const char indx_html[] = R"rawliteral(
<html>
<!DOCTYPE HTML>
<head>
<title> Keg Monitor </title>
<meta name="viewport" content="width=590, user-scalable=no">
<style>
  .button {color: white; background-color: darkblue; text-align: center; display: inline-block; height: 28px; font-size: 20px; margin: 1px 10px 1px 10px; cursor: pointer; }
  .button1 {color: black; background-color: yellow; text-align: center; display: inline-block; height: 28px; font-size: 20px; margin: 1px 10px 1px 10px; cursor: pointer; }
  .button2 {color: white; background-color: red; text-align: center; display: inline-block; height: 28px; font-size: 20px; margin: 1px 10px 1px 10px; cursor: pointer; }
  .warning {font-size: 25px; color: red; margin: 1px 10px 1px 10px; }
  .units {font-size: 25px; color: brown; }
  .labels {font-size: 25px; color: darkslateblue; }
  .values {font-size: 25px; color: blue; }
  body { margin: 8px; background-color: white; font-family: arial;} 
  button {height: 40px; font-size: 25px;}
  form {margin: 1px 10px 1px 10px}
  .input { text-align: right; height: 25px; font-size: 20px;}
  .disabled { text-align: center; margin: 1px 10px 1px 10px height: 25px; font-size: 20px;}
  h1 {font-size: 30px; color: blue; font-family: arial; margin-top: 1px; margin-bottom: 1px; text-align: center; }
  h2 {font-size: 30px; color: DarkOliveGreen; font-family: arial; margin: 5px 1px 1px 1px; }
  h3 {font-size: 25px; color: brown; font-family: arial; margin-top: 1px; margin-bottom: 1px; } 
  p {font-size: 25px; font-family: arial; line-height: 1; margin: 0px 0px 0px 0px; }
  p1 {font-size: 18px; font-family: Courier New; line-height: 1; margin: 0px 0px 0px 0px; }
</style>
</head>
<body>  

<!-- ---------------------------- Start of Page ----------------------- -->
<body>
<h1>Keg Monitor </h1>
<h1><span class="values">%DT Date and Time                                                         </span><br>
<!-- Temperature -->  
  <span class="labels";>Average Temperature = </span>
  <span class="values" id="temperature">%T0                      </span> 
</h1>
<h2>Glasses of Beer </h2> 
<!-- ------ Display Glasses of Beer Table ------- --> 
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px; overflow:hidden;padding:1px 3px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px; font-weight:normal;overflow:hidden;padding:0px 5px;word-break:normal;}
.tg .tg-u2v3{background-color:#ffffff;border-color:#ffffff;color:#3531ff;font-family:Arial, Helvetica, sans-serif !important; font-size:26px;font-weight:bold;text-align:left;vertical-align:top}
.tg .tg-l1w3{border-color:#ffffff;font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:left; vertical-align:top}
.tg .tg-zqq7{background-color:#ffffff;border-color:#ffffff;color:#3531ff;font-family:Arial, Helvetica, sans-serif !important; font-size:26px;font-weight:bold;text-align:center;vertical-align:top}
.tg .tg-dlmr{background-color:#ffffff;border-color:#ffffff;color:#963400;font-family:Arial, Helvetica, sans-serif !important; font-size:26px;font-weight:bold;text-align:left;vertical-align:top}
.tg .tg-oapn{background-color:#ffffff; color:#D2691E; border-color:#ffffff; font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:center; vertical-align:top}
.tg .tg-red{background-color:#ffffff; color:red; border-color:#ffffff; font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:right; vertical-align:top}
.tg .tg-green{background-color:#ffffff; color:green; border-color:#ffffff; font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:right; vertical-align:top}
.tg .tg-yellow{background-color:#ffffff; color:#ffcc00; border-color:#ffffff; font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:right; vertical-align:top}
.tg .tg-percent{background-color:#ffffff; color:#A0522D; border-color:#ffffff; font-family:Arial, Helvetica, sans-serif !important;font-size:26px;text-align:right; vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-l1w3"></th>
    <th class="tg-u2v3">Glasses</th>
    <th class="tg-u2v3">Days</th>
	<th class="tg-u2v3">Alc</th>
	<th class="tg-zqq7">Style</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-dlmr">Tap 1</td>
    <td %T1                         </td>
    <td %D1                         </td>
	<td class="tg-percent">%P1  </td>
	<td class="tg-oapn">%N1                     </td>
  </tr>
  <tr>
    <td class="tg-dlmr">Tap 2</td>
    <td %T2                         </td>
	<td %D2                         </td>
    <td class="tg-percent">%P2  </td>
    <td class="tg-oapn">%N2                     </td>
  </tr>
  <tr>
    <td class="tg-dlmr">Tap 3</td>
    <td %T3                         </td>
	<td %D3                         </td>
	<td class="tg-percent">%P3  </td>
    <td class="tg-oapn">%N3                     </td>
  </tr>
  <tr>
    <td class="tg-dlmr">Tap 4</td>
    <td %T4                         </td>
    <td %D4                         </td>
	<td class="tg-percent">%P4  </td>
    <td class="tg-oapn">%N4                     </td>
  </tr>
  <tr>
    <td class="tg-dlmr">Tap 5</td>
    <td %T5                         </td>
    <td %D5                         </td>
	<td class="tg-percent">%P5  </td>
    <td class="tg-oapn">%N5                      </td>
  </tr>
</tbody>
</table><!-- ------ End of Table ------- --> 
<!-- General Status -------- -->   
<h3>%MS                                                                                        </h3>    
<p> 
	<form method="/get">    
		<span class="labels">RSSI:</span>
		<span class="values" >%RS          </span> 
		<span class="units">dBm &emsp;&emsp;&emsp;&emsp; </span>
		<input class="button" type="submit" name="Refresh" value="Page Refresh" /input> 
    </form>
</p>
<p>
<!-- Additional Status Information -->       
  <span class="labels" >%TS High and Low                                           </span><br>
  <span class="labels"> IP address = </span>
  <span class="values" name="IPaddr">%IP              </span><br>
  <span class="units">Revision: %V1                                            </span> <br>
  <span class="labels" %AL >                                                                             <br>
</p>
<br>
<p>
use "/webserial" for debugging <br>
use "/update" to update new software version <br>
</p>
</body>
</html>
)rawliteral";
