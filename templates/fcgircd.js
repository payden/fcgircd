$(document).ready(function() {
    if(location.pathname == "/" || location.pathname == "/index.html") {
        $("#nickname").focus();
        $("#connect_box").keypress(function(e) {
           if(e.keyCode == 13) {
               var id = e.target.id;
               if(id != "connect_btn") {
                   $("#connect_btn").trigger('click');
               }
           }
        });
        $("#connect_btn").click(function() {
            var data = "";
            data += "nickname=" + encodeURI($("#nickname").val());
            data += "&hostname=" + encodeURI($("#hostname").val());
            data += "&port=" + encodeURI($("#port").val());
            $.post("http://"+location.host+"/json", data, function(resp) {
                var json_response = $.parseJSON(resp);
            });
        })
    }
});
