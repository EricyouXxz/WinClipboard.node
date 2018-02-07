//-----------------------------------------------------
//Test Demo code
//load demo addon
//const test = require('./WinClipboard');
//console.log(test.print());
//// ¥Ú”°: 'world'
//console.log('1112+3456=' + test.add(1112, 3456));
//-----------------------------------------------------
var fs = require('fs');

//function readSyn() {
//    process.stdin.pause();
//    var response = fs.readSync(process.stdin.fd, 1000, 0, "utf8");
//    process.stdin.resume();
//    return response[0].trim();
//}

//console.log("Press any key to continue...");
//console.log(readSyn());

//-----------------------------------------------------
//Test WinClipboard addon
const clipboard = require('./WinClipboard');
var clipboardText = clipboard.getClipboardContent();
console.log(clipboardText);

fs.appendFileSync('ClipboadData.txt', clipboardText);