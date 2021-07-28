function convertNmbrToBytes(str , bits){

    var data = parseInt(str)

    var byteArray = [];

    for ( let idx = 0; idx < bits; idx++ ){
        byteArray.push(0);
    }

    for ( let index = 0; index < byteArray.length; index ++ ) {
        var byte_ = data & 0xFF;
        byteArray [ index ] = byte_;
        data = (data - byte_) / 256 ;
    }

    return byteArray;
}

//accepts uint8tArray and converts it to dec ,bin or hex(see 'base' argument) string
function toNumbersString(buffer, base ) {
  return [...new Uint8Array(buffer)].map(x => x.toString(base)).join(' ');
}

function toAscii(data){
    return [...new Uint8Array(data)].map(x => String.fromCharCode(x)).join('')
}
