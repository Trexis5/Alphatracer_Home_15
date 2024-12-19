

	$(".zonehead img").attr("src","data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAiCAYAAAA+stv/AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAK8AAACvABQqw0mAAAAB90RVh0U29mdHdhcmUATWFjcm9tZWRpYSBGaXJld29ya3MgOLVo0ngAAACzSURBVFiF7ZTRDcMgDETPFbO0Y3SOdADP0TluADom+UkjQAlfgP3B/SBk4TsZeJJSgqUepu4eAgQA+R1IVe9xP3XPQuYTKAKQ3HobkIwk4109ABCS32P/qurSOtxD4VjfI01aksUB6wAzOHDXG4CDCQznQNb7kgduOPAcadLS4oB5gJkcuPRwMYFTJDdV/Y00zL+1qn4WBxYHzANYcKDwcjGBUzM4kHnFf4DFAStvAA4e4Q4um0g8E15kSAAAAABJRU5ErkJggg==");	
	$("input").click(function() {$(this).addClass("btPressed");});

	$(".zonehead").click(function() 
	{
		$(this).children("img").removeClass("down up notrans");
		
		if ($(this).next().css("display") == "block")
		{
			$(this).children("img").addClass("up");
			setCookie("zone" + $(".zonehead").index(this),"0",30);		
		}
		else
		{
			$(this).children("img").addClass("down");
			setCookie("zone" + $(".zonehead").index(this),"1",30);		
		}			
		$(this).next().slideToggle("slow");							
	});	
			
  $("#select").click(function () 
  { 			
  	$(".zonehead img").addClass("notrans");  	
		if ($("#actuators").css("display") == "block")
		{
			setCookie("selection","scenes",30);			
  		$("#actuators").slideToggle("slow", function () {$("#scenes").slideToggle("slow");});     			
			$("#select").fadeOut("slow", function () 
				{    							
					$("#select").fadeIn("slow");   					
				}
			);				
  	}
  	else
  	{
			setCookie("selection","actuators",30);			
  		$("#scenes").slideToggle("slow", function () {$("#actuators").slideToggle("slow");});     			
			$("#select").fadeOut("slow", function () 
			{    		
				$("#select").fadeIn("slow");    		
			}
			);				
		}
	});    	


	
	$(".myslider").each(function(index)
	{
		$(this).on(
			{
				slide: function(){
					$(".tooltip")[index].style.opacity = '1';
				},
				change: function()
				{	 
					$(this).children().first().val(Math.round($(this).children().first().val())); 
					setCookie("slider" + index,$(this).children().first().val(),30);
				
					$(".tooltip")[index].style.opacity = '0';
				  $(".tooltip")[index].style.transition = 'opacity 0.6s linear';
				}
			});
	});				

    $('#relbtn').fadeIn(1000);
	$("#relbtn").click(function(){ 
    $("#relbtn").toggleClass("relrot"); 

    setTimeout("self.location.href='get?inputchart=7'",200);
    }
    );	
    $('#relbtnc').fadeIn(1000);
	$("#relbtnc").click(function(){ 
    $("#relbtnc").toggleClass("relrot"); 

    setTimeout("self.location.href='get?inputchart=1'",200);
    }
    );	