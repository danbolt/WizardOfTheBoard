const fs = require('fs')
const sharedStructs = require('shared-structs')
const strings = require('shared-structs/string')
const buffer = require('buffer').Buffer;

const structs = sharedStructs(`
  struct MapData {
    uint8_t activePieces[24];
    uint8_t pieceX[24];
    uint8_t pieceY[24];
    uint8_t pieceType[24];

    uint8_t activeMonsters[8];
    uint8_t monsterType[8];
    uint8_t monsterX[8];
    uint8_t monsterY[8];

    uint8_t puzzleSpotX[8];
    uint8_t puzzleSpotY[8];

    uint8_t numberOfPuzzleSpots;
    uint8_t playerX;
    uint8_t playerY;
    uint8_t playerRotation;

    uint8_t startLevelDialogue[16];

    uint8_t flagA;
    uint8_t flagB;
    uint8_t flagC;
    uint8_t bgm;
  }
`, {
  alignment: {
    MapData: 4
  }
})

const lengthOfAStructInBytes = structs.MapData().rawArrayBuffer.byteLength;

// Load the processed maps into memory and compute their offsets
const processedMapFiles = fs.readdirSync('processed')
const processedMaps = processedMapFiles.map((filename, i) => {
  return {
    data: JSON.parse(fs.readFileSync('processed/' + filename, { encoding: 'utf8' })),
    offset: i * lengthOfAStructInBytes
  }
})

// Convert the maps into their raw buffers
const mapItems = processedMaps.map((map) => {
  const struct = structs.MapData();

  for (let i = 0; i < 24; i++) {
    struct.activePieces[i] = map.data.piecesActive[i];
    struct.pieceX[i] = map.data.pieceX[i];
    struct.pieceY[i] = map.data.pieceY[i];
    struct.pieceType[i] = map.data.pieceType[i];
  }

  for (let i = 0; i < 8; i++) {
    struct.activeMonsters[i] = map.data.activeMonsters[i];
    struct.monsterType[i] = map.data.monsterType[i];
    struct.monsterX[i] = map.data.monsterX[i];
    struct.monsterY[i] = map.data.monsterY[i];
  }

  struct.numberOfPuzzleSpots = map.data.numberOfPuzzleSpots;
  for (let i = 0; i < map.data.numberOfPuzzleSpots; i++) {
    struct.puzzleSpotX[i] = map.data.puzzleSpotX[i];
    struct.puzzleSpotY[i] = map.data.puzzleSpotY[i];
  }

  struct.playerX = map.data.playerX;
  struct.playerY = map.data.playerY;
  struct.playerRotation = map.data.playerRotation;

  strings.encode(map.data.startLevelDialogue.substring(0, 15), struct.startLevelDialogue);
  
  struct.flagA = map.data.flagA;
  struct.flagB = map.data.flagB;
  struct.flagC = map.data.flagC;
  struct.bgm = map.data.bgm;

  return struct;
})
const mapBuffers = mapItems.map((struct) => { return struct.rawBuffer });

const combinedMapBuffers = buffer.concat(mapBuffers);
fs.writeFile('mapbuffers.bin', combinedMapBuffers, {}, (err) => {
  if (err) {
    console.log(err);
  } else {
    console.log('Finished writing ' + mapBuffers.length + ' map buffers!');
  }
})

let gperfData = `%{#include "maplookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

%}
struct dialogueMappingData;
%%
`;
processedMaps.forEach((mappingItem) => {
  gperfData += mappingItem.data.name + ',' + mappingItem.offset + '\n';
})
fs.writeFile('map-gperf-mapping', gperfData, {}, (err) => {
  if (err) {
    console.log(err);
  } else {
    console.log('Finished writing gperf info for maps!');
  }
})