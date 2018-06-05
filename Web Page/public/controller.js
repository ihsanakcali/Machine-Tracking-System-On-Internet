//Firebase üzerinden gelen verileri çekiyoruz.
var tractionDatas = firebase.database().ref('Makine Takip Sistemi');
tractionDatas.on('value', function(snapshot) {
  var tractionData = snapshot.val();
  document.getElementById("current").innerHTML = tractionData.current + " mA";
  document.getElementById("rpm").innerHTML = tractionData.rpm;
  document.getElementById("temperature").innerHTML = tractionData.temp + " ℃";
  if(tractionData.vib == 0) {
    document.getElementById("vibration").innerHTML = "Titreşim Yok";
  } else if(tractionData.vib == 1) {
    document.getElementById("vibration").innerHTML = "Düşük Titreşim";
  } else if(tractionData.vib == 2) {
    document.getElementById("vibration").innerHTML = "Yüksek Titreşim";
  }
  
});