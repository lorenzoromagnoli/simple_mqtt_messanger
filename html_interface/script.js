var client = mqtt.connect('mqtt://ebd3575a:39bccc93d8b275b1@broker.shiftr.io', {
  clientId: 'javascript'
});

client.on('connect', function(){
  console.log('client has connected!');
  // client.subscribe('/example');
  // client.unsubscribe('/example');
});

client.on('message', function(topic, message) {
  console.log('new message:', topic, message.toString());
});


var form = document.getElementById("form-id");

document.getElementById("your-id").addEventListener("click", function () {
	var message=document.getElementById("message").value;
	client.publish('/display', message);

});
