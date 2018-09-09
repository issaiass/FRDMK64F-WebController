var json_sample = [];	


function getData() {
        $.getJSON("loc.cgx", function(loc) { json_sample = loc; });
        $('#val_x').val(json_sample.x);
        $('#val_y').val(json_sample.y);
        $('#val_z').val(json_sample.z);
        $('#val_ax').val(json_sample.ax);
        $('#val_ay').val(json_sample.ay);
        $('#val_az').val(json_sample.az);
    }
     
function update() {
        getData();
        setTimeout(update, 300);
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
});