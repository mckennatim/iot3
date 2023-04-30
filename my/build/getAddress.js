import { apploc } from "./appdata.js";

const {address}= apploc
console.log('address: ', address);

const searchq = `https://api.geoapify.com/v1/geocode/search?text=${encodeURIComponent(address)}&apiKey=d683f58bb9f348a5945a5e7af5d9f2ab`


var requestOptions = {
  method: 'GET',
};

let addressinfo = {}

const fetchAddress=(addrstr)=>{
  const searchq = `https://api.geoapify.com/v1/geocode/search?text=${encodeURIComponent(addrstr)}&apiKey=d683f58bb9f348a5945a5e7af5d9f2ab`
  return(
    fetch(searchq, {method: 'GET',})
    .then(response => response.json())
    .then(result => {
      const props = result.features[0].properties
      const{lat, lon, timezone}=props
      const tzone= timezone.name
      addressinfo = props
      const locid = addrstr.split(',')[0].replaceAll(" ","")
      console.log('timezone: ', timezone.name);
      return {locid, lat,lon,tzone}
    })
    .catch(error => {
      console.log('error', error)
      addressinfo= error
      return {qmessage: error.message}
    })
  )
}

// fetch(searchq, {method: 'GET',})
//   .then(response => response.json())
//   .then(result => {
//     const props = result.features[0].properties
//     const{lat, lon, timezone}=props
//     addressinfo = props
//     console.log('timezone: ', timezone.name);
//   })
//   .catch(error => {
//     console.log('error', error)
//     addressinfo= error
//   });

export {fetchAddress}

  // fetch("https://api.geoapify.com/v1/geocode/search?text=38%20Upper%20Montagu%20Street%2C%20Westminster%20W1H%201LJ%2C%20United%20Kingdom&apiKey=d683f58bb9f348a5945a5e7af5d9f2ab", requestOptions)
  // .then(response => response.json())
  // .then(result => console.log(result))
  // .catch(error => console.log('error', error));

