xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 21;
 32.93736;13.19558;-4.25124;,
 32.93736;13.19558;15.35370;,
 32.93736;-0.49458;15.35370;,
 32.93736;-0.49458;-4.25124;,
 -32.93736;13.19558;15.35370;,
 -32.93736;-0.49458;15.35370;,
 -32.93736;13.19558;6.68672;,
 -32.93736;-0.49458;6.68672;,
 -21.99941;13.19558;-4.25124;,
 -21.99941;-0.49458;-4.25124;,
 22.68239;11.79190;-5.38727;,
 0.85233;7.32670;-32.29887;,
 16.53039;10.98643;-25.94858;,
 16.59119;3.18007;-21.59982;,
 0.91318;-0.47973;-27.95010;,
 -5.47269;31.00648;4.99612;,
 -21.15075;27.34671;-1.35418;,
 -21.07132;17.15164;4.32532;,
 -5.39324;20.81135;10.67561;,
 15.35386;14.57382;-25.11142;,
 -0.32419;10.91411;-31.46163;;
 
 20;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 3;4,1,6;,
 3;7,9,5;,
 4;5,9,3,2;,
 3;10,1,0;,
 3;10,0,3;,
 4;8,6,1,10;,
 4;8,10,3,9;,
 4;11,12,13,14;,
 4;15,16,17,18;,
 4;16,15,19,20;,
 4;14,13,18,17;,
 4;19,12,11,20;,
 3;12,19,13;,
 3;20,11,14;,
 4;18,13,19,15;,
 4;17,16,20,14;;
 
 MeshMaterialList {
  2;
  20;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.700000;0.700000;0.700000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\stone_00037.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\stone_00037.jpg";
   }
  }
 }
 MeshNormals {
  26;
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.388638;-0.024883;-0.921054;,
  -0.923880;0.000000;-0.382683;,
  -0.060681;0.997946;-0.020548;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.040431;0.999089;-0.013691;,
  0.007312;0.999763;-0.020491;,
  0.000000;-1.000000;0.000000;,
  -0.135614;0.990762;0.000000;,
  0.048796;-0.023093;-0.998542;,
  0.110105;0.000000;-0.993920;,
  0.014621;0.999053;-0.040971;,
  0.423446;-0.438354;-0.792805;,
  -0.423446;0.438356;0.792804;,
  0.388607;-0.070681;-0.918689;,
  0.154990;0.640761;-0.751933;,
  -0.008451;0.875267;-0.483566;,
  0.008345;-0.875159;0.483763;,
  0.905890;0.211469;0.366939;,
  0.882454;0.228980;0.410906;,
  -0.882458;-0.228978;-0.410897;,
  -0.905899;-0.211464;-0.366919;,
  0.856556;0.245853;0.453727;,
  -0.856555;-0.245853;-0.453728;;
  20;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;5,3,3,5;,
  4;3,2,2,3;,
  3;6,7,8;,
  3;9,9,9;,
  4;9,9,9,9;,
  3;4,7,10;,
  3;11,12,11;,
  4;13,8,7,4;,
  4;2,11,11,2;,
  4;16,16,14,14;,
  4;15,15,15,15;,
  4;18,18,17,17;,
  4;19,19,19,19;,
  4;17,16,16,17;,
  3;20,21,21;,
  3;22,23,22;,
  4;24,21,21,24;,
  4;25,25,22,22;;
 }
 MeshTextureCoords {
  21;
  0.973138;0.735267;,
  0.507504;0.735267;,
  0.619981;0.735267;,
  0.795645;0.735267;,
  0.507504;0.264733;,
  0.619981;0.264733;,
  0.517175;0.264733;,
  0.680872;0.264733;,
  0.973138;0.342861;,
  0.795645;0.342861;,
  0.940124;0.662017;,
  0.478337;0.495255;,
  0.379752;0.490838;,
  0.399735;0.476831;,
  0.476191;0.481248;,
  0.366610;0.199139;,
  0.548746;0.203556;,
  0.515152;0.185263;,
  0.408254;0.180846;,
  0.369893;0.476081;,
  0.480303;0.480498;;
 }
}
