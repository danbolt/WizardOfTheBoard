const fs = require('fs')
const sharedStructs = require('shared-structs')
const strings = require('shared-structs/string')
const buffer = require('buffer').Buffer;

const structs = sharedStructs(`
  struct CutsceneInfo {
    uint8_t dialogue[16];

    uint8_t imageKey1[16];
    uint8_t imageKey2[16];
    uint8_t imageKey3[16];

    int32_t bgmIndex;
  }
`, {
  alignment: {
    CutsceneInfo: 4
  }
})

const lengthOfAStructInBytes = structs.CutsceneInfo().rawArrayBuffer.byteLength;

// Load the cutscenesinto memory and compute their offsets
const cutsceneFiles = fs.readdirSync('source')
const processedCutscenes = cutsceneFiles.map((filename, i) => {
  return {
    data: JSON.parse(fs.readFileSync('source/' + filename, { encoding: 'utf8' })),
    offset: i * lengthOfAStructInBytes
  }
})


// Endianness flip from:
// https://stackoverflow.com/questions/5320439/how-do-i-swap-endian-ness-byte-order-of-a-variable-in-javascript
function swap32(val) {
    return ((val & 0xFF) << 24)
           | ((val & 0xFF00) << 8)
           | ((val >> 8) & 0xFF00)
           | ((val >> 24) & 0xFF);
}

// Convert the cutscenes into their raw buffers
const cutsceneItems = processedCutscenes.map((map, i) => {

  map.data.name = cutsceneFiles[i].split('.')[0]

  const struct = structs.CutsceneInfo();


  strings.encode(map.data.dialogue.substring(0, 15), struct.dialogue);
  strings.encode(map.data.imageKey1.substring(0, 15), struct.imageKey1);
  strings.encode(map.data.imageKey2.substring(0, 15), struct.imageKey2);
  strings.encode(map.data.imageKey3.substring(0, 15), struct.imageKey3);
  
  struct.bgmIndex = swap32(map.data.bgm);

  return struct;
})
const cutsceneBuffers = cutsceneItems.map((struct) => { return struct.rawBuffer });

const combinedCutsceneBuffers = buffer.concat(cutsceneBuffers);
fs.writeFile('cutscenebuffers.bin', combinedCutsceneBuffers, {}, (err) => {
  if (err) {
    console.log(err);
  } else {
    console.log('Finished writing ' + cutsceneBuffers.length + ' cutscene buffers!');
  }
})

let gperfData = `%{#include "cutscenelookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

%}
struct cutsceneMappingData;
%%
`;
processedCutscenes.forEach((mappingItem) => {
  gperfData += mappingItem.data.name + ',' + mappingItem.offset + '\n';
})
fs.writeFile('cutscene-gperf-mapping', gperfData, {}, (err) => {
  if (err) {
    console.log(err);
  } else {
    console.log('Finished writing gperf info for maps!');
  }
})