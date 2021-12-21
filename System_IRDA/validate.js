function refresh(cname){
  if(document.cookie!=0){///funkcja odświeżająca ciastko po wykonaniu akcji lub odświeżeniu strony
  const now = new Date();
    now.getTime();
 let ciacho = decodeURIComponent(document.cookie);
let acc = ciacho.split(';');
let exp = acc[1];
if(exp-now>0){
    exp.setTime(now.getTime()+(60*10000));
    let expire = "expires="+document.toGMTString();
    document.cookie= "username="+cname+";"+expire+";path=/";
}
else{
 //   document.cookie= "username=;";
    alert("You have been away to long")
}
  }


}

function if_Login_is_correct(username){
let actual= new Date();
actual.setTime(actual.getTime()+(60*10000));
document.cookie="username="+username+";"+"expires="+actual.toGMTString()+";path=/";
}
/// logowanie do systemu



