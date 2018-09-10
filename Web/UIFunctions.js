var json_sample = [];
var json_rly = [];
	
function getData() {
    $.getJSON("loc.cgx", function(loc) { json_sample = loc; });
    $('#val_x').val(json_sample.x);
    $('#val_y').val(json_sample.y);
    $('#val_z').val(json_sample.z);
    $('#val_ax').val(json_sample.ax);
    $('#val_ay').val(json_sample.ay);
    $('#val_az').val(json_sample.az);
}
 
function getRelays() {
    $.getJSON("rly.cgx", function(rly) { json_rly = rly; });
    $('#val_rly1').val(json_rly.rly1);
    if (json_rly.rly1 == "0") {
				$('#img_rly1').attr('src', 'on.png');
		} else {
				$('#img_rly1').attr('src', 'off.png');			
		}
}

	
function update() {
    getData();
    getRelays();
    setTimeout(update, 300);
}
	
function setVal(number){
    var inputs = document.getElementsByTagName("input");

	
    for (var i = 0; i < inputs.length; i++) {
        switch(inputs[i].id) {
            case "rtbtn1":
                document.getElementById("ntbtn1").value = number;                   
                break;
            case "ntbtn1":
                document.getElementById("rtbtn1").value = number;
        }
    }
}
	
$(window).load(function() {
    update();
})

$(document).ready( function() {
		$("#sbtn1").click(function(){
      $.ajax({
            url: 'rpc.cgx', 
            data: JSON.stringify ({jsonrpc:'2.0',method:'set', params:'1',id:"jrpc"} ),  // id is needed !!
            type:"POST",
            dataType:"json",
            //success:  function (data)       { alert("The result is : " + data.result);},
						error: function (err)  { console.log( "JSON rpc.cgx Request Failed: " + err);document.getElementById('btn1').innerHTML = "!JSON RPC ERROR!";},
						timeout: 300
         });
    });
		$("#cbtn1").click(function(){
      $.ajax({
            url: 'rpc.cgx', 
            data: JSON.stringify ({jsonrpc:'2.0',method:'clear',params:'0',id:"jrpc"} ),  // id is needed !!
            type:"POST",
            dataType:"json",
            //success:  function (data)       { alert("The result is : " + data.result);},
            error: function (err)  { console.log( "JSON rpc.cgx Request Failed: " + err);document.getElementById('state').innerHTML = "!JSON RPC ERROR!";},
						timeout: 300
         });
    });
		$("#tbtn1").click(function(){
      $.ajax({
            url: 'rpc.cgx', 
            data: JSON.stringify ({jsonrpc:'2.0',method:'toggle',params:document.getElementById("ntbtn1").value,id:"jrpc"} ),  // id is needed !!
            type:"POST",
            dataType:"json",
            //success:  function (data)       { alert("The result is : " + data.result);},
            error: function (err)  { console.log( "JSON rpc.cgx Request Failed: " + err);document.getElementById('state').innerHTML = "!JSON RPC ERROR!";},
						timeout: 300
         });
    });
});