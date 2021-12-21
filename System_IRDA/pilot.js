function pilotek(value){
        let header=" <meta charset=\"UTF-8\">\
        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        <title>ESP8266 Remote Controller</title>"
    switch(value)
    {
        case "LED":
            
            document.getElementById("pilot2").innerHTML=" ";
            
            document.head.innerHTML=header
          var linki= document.createElement('style');
        linki.innerHTML="a{color:wheat; text-decoration:none;} \n#model{\n    position:absolute;\n    top:50px;\n    left:250px;\n}\n#pilot {\n    \n    \n    width: 240px;\n    height: 350px;\n    padding: 10px;\n    margin: 10px, 0,0,0;\n    border: solid black 1px;\n    border-radius: 10px;\n    padding-left: 10px;\n}\n\nbutton {\n\n    margin-right: 10px;\n    margin-bottom: 10px;\n    padding: 0, 0;\n    width: 50px;\n    border-radius: 50%;\n    height: 50px;\n    text-align: center;\n}\n\n#off {\n    background-color: black;\n    color: white;\n}\n\n#on {\n    background-color: red;\n    color: white;\n}\n\n#r {\n    background-color: red;\n}\n\n#g {\n    background-color: green;\n}\n\n#b {\n    background-color: blue;\n}\n\n#w {\n    background-color: grey;\n}\n\n#o1 {\n    background-color: rgb(245, 131, 94);\n}\n\n#o2 {\n    background-color: rgb(244, 115, 119);\n}\n\n#o3 {\n    background-color: rgb(241, 160, 89);\n}\n\n#o4 {\n    background-color: rgb(216, 233, 73);\n}\n\n#g1 {\n    background-color: rgb(71, 219, 71);\n}\n\n#g2 {\n    background-color: rgb(71, 192, 239);\n}\n\n#g3 {\n    background-color: rgb(64, 114, 164);\n}\n\n#g4 {\n    background-color: rgb(64, 95, 140);\n}\n\n#b1 {\n    background-color: rgb(74, 98, 225);\n}\n\n#b2 {\n    background-color: rgb(101, 31, 118);\n}\n\n#b3 {\n    background-color: rgb(150, 45, 130);\n}\n\n#b4 {\n    background-color: rgb(210, 130, 210);\n}";
          document.head.appendChild(linki);
         // var pos=document.head.innerHTML;
           // document.head.innerHTML=header+pos         
             var buttons = [
        "plus", "min", "off", "on", "r", "g", "b", "w", "o1", "g1", "b1", "fl", "o2", "g2", "b2", "str", "o3", "g3", "b3", "fd", "o4", "g4", "b4", "smh"
    ]
        var cont = document.getElementById('pilot2')
        insertswitch("LED");
        var i = 0;
        for (k = 0; k < buttons.length; k++)
        {
            i++;

            var tmp = document.createElement('button')
            tmp.setAttribute('name', 'com')
            tmp.setAttribute('value', buttons[k])
            tmp.setAttribute('id', buttons[k])
            tmp.innerText = buttons[k];
            cont.appendChild(tmp)

        }
        

       break;
       case "PHILIPS":
        
        document.getElementById("pilot2").innerHTML=" ";
        
        document.head.innerHTML=header;
        var linki= document.createElement('style');
        
        
        linki.innerHTML="#model{\nposition:absolute;\ntop:50px;\nleft:250px;\n\n}\n#pilot\n{\n    display:block;\nmargin: 0;\npadding-left: 10px;\n\nbackground-color: #0F0F0F;\nwidth:200px;\nheight:700px;\nborder-radius:10px;\n}\nbutton{\n   \n    background-color: #AFAFAF\n}\nbutton:hover{\n    background-color:greenyellow;\n}\n#Toggle{\n\n   border-radius: 100%;\n  width:25px;\n  height:25px;\n   position:relative;\n   top:25px;\n   left:85px;\n   \n}\n#Stop{border-radius:10px; position:relative;left:-30px; top:75px;width:60px;margin:5px 0 5px 5px;}\n#Pause{border-radius:10px; position:relative;left:-27px; top:75px;width:60px;margin:5px 0 0 5px;}\n#Record{border-radius:10px; position:relative;left:130px; top:44px;width:60px;margin:5px 0 0 5px;}\n\n#Rewind{border-radius:10px; position:relative;left:-70px; top:75px;width:60px;margin:5px 0 0 5px;}\n#Play{border-radius:10px; position:relative;left:-67px; top:75px;width:60px;margin:5px 0 0 5px;}\n#Forward{border-radius:10px; position:relative;left:130px; top:49px;width:60px;margin: 5px 0 0 5px;}\n\n#Guide{border-radius:10px; position:relative;left:-70px; top:79px;width:60px;margin: 5px 0 0 5px;}\n#Setup{border-radius:10px; position:relative;left:-67px; top:79px;width:60px;margin: 5px 0 0 5px;}\n#Format{border-radius:10px; position:relative;left:130px; top:39px;width:60px;margin:5px 0 0 5px;}\n\n#Source{border-radius:100%; position:relative;left:-60px; top:80px;width:40px; height:40px;margin:5px 0 0 5px;}\n#TV{border-radius:100%; position:relative;left:-43px; top:80px;width:50px;height:50px;margin:5px 0 0 5px;}\n#Exit{border-radius:100%; position:relative;left:135px; top:30px;width:40px;height:40px;margin:5px 0 0 5px;}\n\n#Red{position:relative;top:90px;left:-45px; width:43px;border-radius:5px; }\n#Green{position:relative;top:90px; left:-38px;width:43px;border-radius:5px;}\n#Yellow{position:relative;top:90px;width:43px; left:-33px;border-radius:5px;}\n#Blue{position:relative;top:-13px;width:43px;left:145px;border-radius:5px;}\n\n\n#Info{position:relative;top:100px;left:-40px; width:0;height:0;background-color: transparent; border-top:75px solid #3F3F3F; border-right: 70px solid transparent; text-align:center;z-index: 1;}\n#Up{position:relative;top:-70px;left:49px;;width:0;height: 0;background-color: transparent;border-left:40px solid transparent; border-right: solid transparent 40px; border-bottom: 45px solid #AFAFAF;z-index: 2;}\n#Options{position:relative;top:5px;left:10px;width:0;height:0;background-color: transparent; border-top:75px solid #3F3F3F; border-left: 70px solid transparent; text-align:center;z-index: 1;}\n#Info:hover{\n    border-top:75px solid #00ffff; border-right: 70px solid transparent;\n}\n#Options:hover{\n    border-top:75px solid #00ffff; border-left: 70px solid transparent;\n}\n#Up:hover{\n    border-left:40px solid transparent; border-right: solid transparent 40px; border-bottom: 45px solid greenyellow\n}\n    #Left{position:relative;top:-70px; width:0;height: 0;background-color: transparent;border-top:40px solid transparent; border-bottom: solid transparent 40px; border-right: 35px solid #AFAFAF; z-index: 1;}\n    #OK{position:relative;top:-72px;left:7px;border-radius: 100%; width:80px;height: 80px;z-index: 1;}\n    #Right{position:relative;top:-70px;left:12px;width:0;height: 0;background-color: transparent;border-top:40px solid transparent; border-bottom: solid transparent 40px; border-left: 35px solid #AFAFAF;z-index: 1;}\n    \n    #Back{position:relative;top:5px;}\n    #Down{position:relative;top:5px;}\n    #List{position:relative;top:5px;}\n\n    #channel_plus{position:relative;top:5px;}\n    #channel_minus{position:relative;top:5px;}\n    #Vol_down{position:relative;top:5px;}\n    #Mute{position:relative;top:5px;}\n    #Vol_up{position:relative;top:5px;}\n   \n\n\n#_1{position:relative;top:-20px;left:-100px; width:55px;}\n#_2{position:relative;top:-20px; left: 65px;width:55px;}\n#_3{position:relative;top:-20px;width:55px;}\n#_4{position:relative;top:-20px;width:55px;}\n#_5{position:relative;top:-20px;width:55px;}\n#_6{position:relative;top:-20px;width:55px;}\n#_7{position:relative;top:-20px;width:55px;}\n#_8{position:relative;top:-20px;width:55px;}\n#_9{position:relative;top:-20px;width:55px;}\n#_0{position:relative;top:-20px;width:55px;}\n#Subtitle{position:relative;top:-20px;width:55px;}\n#Text{position:relative;top:-20px;width:55px;}"
        document.head.appendChild(linki);
       // var pos=document.head.innerHTML;
         // document.head.innerHTML=item 
           var buttons =[
               "Toggle", "Stop", "Pause","Record","Rewind","Play","Forward","Guide","Setup","Format","Source","TV","Exit","Red","Green","Yellow","Blue","Info","Up","Options","Left","OK","Right","Back","Down","List",
               "channel_plus","channel_minus","Vol_down","Mute","Vol_up","_1","_2","_3","_4","_5","_6","_7","_8","_9","Subtitle","_0","Text"
            
           ]
           var cont= document.getElementById("pilot2")
           insertswitch("PHILIPS");
           for(i=0;i<buttons.length;i++)
       {
        var tmp = document.createElement('button')
        tmp.setAttribute('name', 'com')
        tmp.setAttribute('value', buttons[i])
        tmp.setAttribute('id', buttons[i])
        tmp.innerText = buttons[i];
        cont.appendChild(tmp)

       }
       
       
       break; 
       case "ORANGE":
        document.getElementById("pilot2").innerHTML=" ";
        
        document.head.innerHTML=header;
      //  var linki= document.createElement('link');
       // linki.rel="stylesheet";
     //   linki.href="Orange.css"
       // linki.type="text/css"
     //   document.head.appendChild(linki);
           {var buttons=[
               "Toggle","Tv_Toggle","_1","_2","_3","_4","_5","_6","_7","_8","_9","Fav","_0","Prev","Vol_up","Vol_down","Mute","List",
               "Prog_up","Prog_down","Vod","Menu","Prog_L","Pvr","Up","Info","Left","Ok","Right","Return","Down","Exit","Red","Green","Yellow",
               "Blue","Message","Audio","Subtitle","Record","Rewind","Play_Pause","Stop","Forward"
        ]
        var cont= document.getElementById("pilot2")
           insertswitch("ORANGE");
           for(i=0;i<buttons.length;i++)
       {
        var tmp = document.createElement('button')
        tmp.setAttribute('name', 'com')
        tmp.setAttribute('value', buttons[i])
        tmp.setAttribute('id', buttons[i])
        tmp.innerText = buttons[i];
        cont.appendChild(tmp)

       }
                        }
    }

    //window.addEventListener('DOMContentLoaded', (LOADED) => { console.log("LOADED"); btn_f() });
}
function command(){
var xhr = new XMLHttpRequest();
xhr.open("POST",'send.html',true);
xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
xhr.onreadystatechange=function(){
if(this.readyState===XMLHttpRequest.DONE && this.status === 200){}
}
xhr.send(FormData);
}
function insertswitch(dft){
   models=["","LED","PHILIPS","SONY","ORANGE","AC","SHARP","LG"]
    var r=models.findIndex(Element=> Element ==dft)
    models[0]=dft;
    models[r]="";
    var form=document.getElementById('pilot2')
    var swf=document.createElement('select')
    swf.setAttribute('id','model')
    swf.setAttribute('onchange','var war=document.getElementById(\'model\').value;   pilotek(war)')
   swf.setAttribute('name','model')
    for(j=0;j<models.length;j++){
        var opt=document.createElement('option')
    
        opt.setAttribute('value',models[j])
        opt.innerText=models[j]
        swf.appendChild(opt)
    }
    form.appendChild(swf);
   // swf.innerText=this.innerText+dft
}