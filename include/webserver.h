// Use %% instead of % in css, it interferes with template plaveholder sign of asynweb
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
	<title>SeismoHA Web Server Configuration</title>
	<meta charset='utf-8'>
	<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no" />
	<link rel="icon" href="data:,">
	<style>		html {
			  font-family: verdana;
		  text-align: center;
		}
		h1 {
		  font-size: 1.8rem;
		  color: white;
		}
		h2{
		  font-size: 1.2rem;
		  color: white;
		}
		.topnav {
		  overflow: hidden;
		  background-color: #1fa3ec;
		  text-align: center;
		}
		div,
		fieldset,
		input,
		select {
			  padding: 5px;
			  font-size: 1em;
		}
		
		input {
			  width: 100%%;
			  box-sizing: border-box;
			  -webkit-box-sizing: border-box;
			  -moz-box-sizing: border-box;
		}
		
		select {
			  width: 100%%;
		}
		
		textarea {
			  resize: none;
			  width: 98%%;
			  height: 318px;
			  padding: 5px;
			  overflow: auto;
		}
		
		body {
			  text-align: center;
			  font-family: verdana;
		}
		
		td {
			  padding: 0px;
		}
		
		button {
			  border: 0;
			  border-radius: 0.3rem;
			background-color: #1fa3ec;
			color: #fff;
			line-height: 2.4rem;
			font-size: 1.2rem;
			width: 100%%;
			-webkit-transition-duration: 0.4s;
			transition-duration: 0.4s;
			cursor: pointer;
		}
		
		button:hover {
			background-color: #0e70a4;
		}
		
		.bred {
			background-color: #d43535;
		}
		
		.bred:hover {
			background-color: #931f1f;
		}
		
		.bgrn {
			  background-color: #47c266;
		}
		
		.bgrn:hover {
			  background-color: #5aaf6f;
		}
		
		a {
			  text-decoration: none;
		}
		
		.p {
			  float: left;
			  text-align: left;
		}
		
		.q {
			  float: right;
			  text-align: right;
		}
	</style>
</head>
<body>
<div class="topnav">
	<h1>
		SeismoHA Web Server
		</h3>
		<h2>
			Configuration
		</h2>
</div>
<div style='text-align:left;display:inline-block;min-width:340px;'>
	<fieldset>
		<form method='get' action='sv'>
			%MQTTPLACEHOLDER% <br/>
			<button type='submit' class='button bgrn'>Save</button> 
		</form>
	</fieldset>
	<br/>
	<br/>
    <button onclick="window.location.href='/update';">
      OTA Firmware Update</button>
	<br/>
	<br/>
	<form action='dt' method='get'>
		<button>RTC Update Time</button>
	</form>
	<br/>
	<br/>
	<form action='rt' method='get' onsubmit='return confirm("Confirm Reset Configuration");'>
		<button class='button bred'>Reset Configuration</button>
	</form>
	<br/>
	<br/>
</div>
<script>
	function pushButton(element) {
	  var xhr = new XMLHttpRequest();
	  xhr.open("GET", "/update?output="+element.id+"&state=1");
	  xhr.send();
	}
</script>
</body>
</html>
)rawliteral";