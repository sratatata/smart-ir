(function(){
  var emmit = function(type, code){
    $.ajax("emmit?code="+code+"&type="+type, {
      done: function(data){
        alert(data);
      },
      fail: function(){
        alert("Error");
      }
    });

  }

  $("a.button").click(function(){
    var code = $(this).data("code");
    var type = $(this).parent().parent().data("type");
    emmit(type, code);
  });
})();
