$(document).ready(function() {
    if(location.pathname == "/" || location.pathname == "/index.html") {
        $("#nickname").focus();
        $("#connect_box").keypress(function(e) {
           if(e.keyCode == 13) {
               //TODO: make sure event target is not an enter fired on the button which would cause two 'clicks'
               $("#connect_btn").trigger('click');
           }
        });
        $("#connect_btn").click(function() {
            console.log("Fire connect.");
        })
    }
});
