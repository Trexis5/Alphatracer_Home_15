var
	num = 0;
	bcnt = 0; 
                   
   
  function loadDoc2(){
     

        intervall_running = true;
        loadValues();  
             
        
    }
   
   
   
   
   
    function onBodyLoad(){
      
        running = true;
        myInterval2 = setInterval(loadValues, 60000);  intervall_running = true;
     
    }
    
    

   function loadValues(){

      var xh = new XMLHttpRequest();
      xh.onreadystatechange = function(){
        if (xh.readyState == 4){
          if(xh.status == 200) {
            var res = JSON.parse(xh.responseText);

             //vsize.add(res.size);
             //sendzeiger = parseInt(res.sz);
             //writezeiger = parseInt(res.wz);  
             document.getElementById("injava2").value = res.responseText; 

          };
        }
      };
      

        xh.open("GET", "/all3", true);
        xh.send(null);

    };
