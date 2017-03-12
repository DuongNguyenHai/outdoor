window.onload = function() {

var obj;

function loadXMLDoc(file) {
  var xhttp = new XMLHttpRequest();
  var obj;
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      obj = JSON.parse(this.responseText);
    }
  };
  xhttp.open("GET", file, false);
  xhttp.send();
  return obj;
}

var obj = loadXMLDoc("system.txt");
document.getElementById("switchLamp").checked = obj.lamp.state;
// document.getElementById("switchHeater").checked=obj.measure_tempt_fishtank.state;
// document.getElementById("switchBumper").checked=false;
var tm = new Date(obj.system_time);
document.getElementById("systemTime").innerHTML = tm.getHours() +'h' + tm.getMinutes();
document.getElementById("lightOnFrom").value = obj.lamp.on;
document.getElementById("lightOnTo").value = obj.lamp.off;
document.getElementById("temptMeasure").value = obj.measure_tempt_fishtank.schedule;
document.getElementById("input-datetime").value = obj.system_time;

var objTempt = loadXMLDoc("tempt.txt");
document.getElementById("temptTank").innerHTML = objTempt.tempt.current;
var valTemptGraph;
if(objTempt.tempt.log.length>56)
  valTemptGraph = objTempt.tempt.log.slice(objTempt.tempt.log.length-8, objTempt.tempt.log.length);
else
  valTemptGraph = objTempt.tempt.log;


// render graph of temperature

var graphTemptWidth = document.getElementById('graph-tempt').offsetWidth;
graphTemptWidth = graphTemptWidth - 85 - 8;
var temptGraph = new Easygraphs({
    container: document.getElementById('tempt-graph'),
    width: graphTemptWidth,
    height: 200,
    padding: {
      top: 30,
      right: 30,
      left: 55
    },
    tooltip: {
      template: '{{ value }}',
      widthAuto: true,
      color: '#222',
      background: '#FFF',
    },
    yAxis: {
      text: {
        toFixed: 0
      },
      title: {
        text: 'Tuần 1'
      },
      grid: {
        dasharray: '1%'
      }
    },
    xAxis: {
      labels: [''],
      grid: {
        dasharray: '1%'
      }
    },
    data: [{
      name: 'nhiệt độ',
      dots: {
        color: 'rgb(246, 75, 47)'
      },
      line: {
        width: 2,
        color: 'rgb(246, 75, 47)',
        fill: 'rgba(246, 75, 47, 0.5)'
      },
      values: valTemptGraph
    }]
});

temptGraph.render();

// send data to server by AJAX, print respond in alert box
var sendData = document.querySelectorAll(".sendData");

for (var i = 0; i < sendData.length; i++) {
   sendData[i].onclick = function() {
      var formElement = new FormData(this.parentElement); // <form> is parent of button.sendData. its important
      sendRequestFromData(formElement);
   };
}

// send data to server by AJAX, print respond in alert box
var switchData = document.querySelectorAll(".switchData");

for (var i = 0; i < switchData.length; i++) {
   switchData[i].onclick = function() {
      var inputSwitch = this.previousElementSibling;
      var formElement = new FormData();
      if(!inputSwitch.checked)
         formElement.append(inputSwitch.name, 'ON');
      else
         formElement.append(inputSwitch.name, 'OFF');

      sendRequestFromData(formElement);
   };
}

document.getElementById("dateTime").onclick = function() {
   var formElement = new FormData(this.parentElement); // <form> is parent of button.sendData. its important
   var timeUnix = new Date(formElement.get("date")).getTime(); // get ISO time from "formElement" by name="date". then, convert to unix time
   console.log(timeUnix);
   sendRequest("date=" + timeUnix);
}

function sendRequestFromData(formData) {
   var request = new XMLHttpRequest();
   request.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
         alertRespond(this.responseText);
      }
   };

   // Display the key/value pairs
   for(var pair of formData.entries()) {
      console.log(pair[0]+ ', '+ pair[1]);
   }
   request.open("POST", "/set");
   request.send(formData);
}

function sendRequest(data) {
   var request = new XMLHttpRequest();
   request.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
         alertRespond(this.responseText);
      }
   };
   request.open("POST", "/set");
   request.send(data);
}

// print data in alert-box
function alertRespond(str) {
   var alertType, alertData, alertRes, alertStr;
   var responType = str.charAt(0);
   var responData = str.substr(2, str.length);
   switch (responType) {
      case "S": { // success
         alertType = "alert-success";
         alertRes = "Thành công";
      }break;
      case "W": { // warning
         alertType = "alert-warning";
         alertRes = "Cảnh báo";
      } break;
      case "D": { // danger or fail
         alertType = "alert-danger";
         alertRes = "Thất bại";
      } break;
      default: { // info
         alertType = "alert-info";
         alertRes = "Thông báo";
      }

   }

   alertStr = '<div class="alert '+ alertType +'"><strong> '+ alertRes +'</strong>. <span>'+ responData +'</span><a href="#" class="close">×</a></div>';
   var field = document.getElementById('field-alert');
   field.innerHTML += alertStr;
   var alert = document.querySelectorAll(".alert");
   for (var i = 0; i < alert.length; i++) {
      alert[i].onclick = function() {
         this.style.opacity = 0;
         this.style.zIndex = -1;
         var This = this;
         window.setTimeout(function() {field.removeChild(This);},1000);
      }
   }
   // automatic remove alert
   window.setTimeout(function(){
      var child = field.childNodes;
      for (var i = 0; i < child.length; i++) {
         child[i].parentElement.removeChild(child[i]);
      }
   },7000);
}




window.onresize = function(event) {

};


};
