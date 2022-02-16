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
 92;
 -0.66940;-2.78673;-2.48924;,
 -2.11595;-2.78673;-2.48924;,
 -2.11595;-2.15793;-2.48924;,
 -0.66940;-2.15793;-2.48924;,
 -2.11595;2.24659;-2.48924;,
 -0.66940;2.24659;-2.48924;,
 -2.11595;2.87585;-2.48924;,
 -0.66940;2.87585;-2.48924;,
 -8.72153;-2.54361;-2.28200;,
 -8.72153;-1.96878;-2.28200;,
 -8.72153;2.05751;-2.28200;,
 -8.72153;2.63274;-2.28200;,
 -12.70995;-2.82937;-2.52539;,
 -12.70995;-2.19110;-2.52376;,
 -12.70995;2.27978;-2.52930;,
 -12.70995;2.91852;-2.52539;,
 -4.73299;2.58160;2.08719;,
 -0.66940;2.87585;2.33802;,
 -8.72153;2.63274;2.13078;,
 -12.70995;2.91852;2.37463;,
 -4.73299;0.89023;2.08719;,
 -0.66940;0.98830;2.33802;,
 -4.73299;-0.80111;2.08719;,
 -0.66940;-0.89920;2.33802;,
 -4.73299;-2.49251;2.08719;,
 -0.66940;-2.78673;2.33802;,
 -8.72153;0.90730;2.13078;,
 -8.72153;-0.81818;2.13078;,
 -8.72153;-2.54361;2.13078;,
 -12.70995;1.00252;2.37463;,
 -12.70995;-0.91339;2.37463;,
 -12.70995;-2.82937;2.37463;,
 -2.11595;2.24659;-1.88142;,
 -2.11595;-2.15793;-1.88142;,
 -8.72153;2.05751;-1.72637;,
 -8.72153;-1.96878;-1.72637;,
 -8.72153;-1.96878;-2.05563;,
 -8.72153;2.05751;-2.05563;,
 -8.72344;0.90730;1.95592;,
 -4.73490;0.89023;1.91233;,
 -8.72344;-0.81818;1.95592;,
 -4.73490;-0.80111;1.91233;,
 1.52256;1.37538;-2.14304;,
 1.73572;0.87176;-2.14304;,
 0.58797;0.53793;-2.14304;,
 0.96052;1.56642;-2.14304;,
 -0.00850;0.87176;-2.14304;,
 -0.00850;1.74009;-2.14304;,
 -0.60499;0.53793;-2.17506;,
 -0.97753;1.56642;-2.14304;,
 -1.75273;0.87176;-2.14304;,
 -1.53956;1.37538;-2.14304;,
 1.92955;0.00343;-2.14304;,
 0.96052;0.00343;-2.14304;,
 -0.00850;0.00343;-2.14304;,
 -0.97753;0.00343;-2.14304;,
 -1.94655;0.00343;-2.14304;,
 1.73572;-0.86491;-2.14304;,
 0.58797;-0.53108;-2.14304;,
 -0.00850;-0.86491;-2.14304;,
 -0.60499;-0.53108;-2.17506;,
 -1.75273;-0.86491;-2.14304;,
 1.52256;-1.36854;-2.14304;,
 0.96052;-1.55955;-2.14304;,
 -0.00850;-1.73319;-2.14304;,
 -0.97753;-1.55955;-2.14304;,
 -1.53956;-1.36854;-2.14304;,
 -1.75273;0.87176;2.03416;,
 -1.53956;1.37538;2.03416;,
 -1.94655;0.00343;2.03416;,
 -1.75273;-0.86491;2.03416;,
 -1.53956;-1.36854;2.03416;,
 -0.60499;0.53793;2.00215;,
 -0.97753;1.56642;2.03416;,
 -0.00850;0.87176;2.03416;,
 -0.00850;1.74009;2.03416;,
 0.58797;0.53793;2.03416;,
 0.96052;1.56642;2.03416;,
 1.73572;0.87176;2.03416;,
 1.52256;1.37538;2.03416;,
 -0.97753;0.00343;2.03416;,
 -0.00850;0.00343;2.03416;,
 0.96052;0.00343;2.03416;,
 1.92955;0.00343;2.03416;,
 -0.60499;-0.53108;2.00215;,
 -0.00850;-0.86491;2.03416;,
 0.58797;-0.53108;2.03416;,
 1.73572;-0.86491;2.03416;,
 -0.97753;-1.55955;2.03416;,
 -0.00850;-1.73319;2.03416;,
 0.96052;-1.55955;2.03416;,
 1.52256;-1.36854;2.03416;;
 
 88;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;1,8,9,2;,
 4;4,10,11,6;,
 4;8,12,13,9;,
 4;10,14,15,11;,
 4;7,6,16,17;,
 4;6,11,18,16;,
 4;11,15,19,18;,
 4;17,16,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;16,18,26,20;,
 4;22,27,28,24;,
 4;18,19,29,26;,
 4;26,29,30,27;,
 4;27,30,31,28;,
 4;25,24,1,0;,
 4;24,28,8,1;,
 4;28,31,12,8;,
 4;25,0,3,23;,
 4;23,3,5,21;,
 4;21,5,7,17;,
 4;12,31,30,13;,
 4;13,30,29,14;,
 4;14,29,19,15;,
 4;32,4,2,33;,
 4;34,10,4,32;,
 4;33,2,9,35;,
 4;33,35,34,32;,
 4;36,13,14,37;,
 3;14,10,37;,
 3;13,36,9;,
 4;34,35,36,37;,
 4;38,39,20,26;,
 4;40,38,26,27;,
 4;41,40,27,22;,
 4;39,41,22,20;,
 4;39,38,40,41;,
 4;42,43,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;49,48,50,51;,
 4;43,52,53,44;,
 4;44,53,54,46;,
 4;46,54,55,48;,
 4;48,55,56,50;,
 4;52,57,58,53;,
 4;53,58,59,54;,
 4;54,59,60,55;,
 4;55,60,61,56;,
 4;57,62,63,58;,
 4;58,63,64,59;,
 4;59,64,65,60;,
 4;60,65,66,61;,
 4;51,50,67,68;,
 4;50,56,69,67;,
 4;56,61,70,69;,
 4;61,66,71,70;,
 4;68,67,72,73;,
 4;73,72,74,75;,
 4;75,74,76,77;,
 4;77,76,78,79;,
 4;67,69,80,72;,
 4;72,80,81,74;,
 4;74,81,82,76;,
 4;76,82,83,78;,
 4;69,70,84,80;,
 4;80,84,85,81;,
 4;81,85,86,82;,
 4;82,86,87,83;,
 4;70,71,88,84;,
 4;84,88,89,85;,
 4;85,89,90,86;,
 4;86,90,91,87;,
 4;79,78,43,42;,
 4;78,83,52,43;,
 4;83,87,57,52;,
 4;87,91,62,57;,
 4;79,42,45,77;,
 4;77,45,47,75;,
 4;75,47,49,73;,
 4;73,49,51,68;,
 4;62,91,90,63;,
 4;63,90,89,64;,
 4;64,89,88,65;,
 4;65,88,71,66;;
 
 MeshMaterialList {
  6;
  88;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.081600;0.081600;0.081600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.207200;0.207200;0.207200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.649600;0.649600;0.649600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.329600;0.627200;0.282400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.132000;0.060000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.429600;0.006400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  96;
  0.000000;0.000000;-1.000000;,
  -0.015681;0.000000;-0.999877;,
  -0.010454;0.000000;-0.999945;,
  -0.010454;0.000000;-0.999945;,
  0.014645;0.000638;-0.999893;,
  0.015145;0.001529;-0.999884;,
  0.060617;0.001275;-0.998160;,
  0.088953;0.000330;-0.996036;,
  0.089452;0.001220;-0.995990;,
  -0.015681;0.000000;-0.999877;,
  0.061616;0.003055;-0.998095;,
  -0.036807;0.999261;0.011030;,
  -0.061609;0.000000;0.998100;,
  -0.061609;0.000000;0.998100;,
  -0.024045;0.999574;0.016515;,
  -0.037452;0.000000;0.999298;,
  0.030123;0.999486;0.011009;,
  0.044339;0.000000;0.999017;,
  0.044339;0.000000;0.999016;,
  0.071469;0.997443;0.000000;,
  0.061026;0.000000;0.998136;,
  0.061026;0.000000;0.998136;,
  -0.061609;0.000000;0.998100;,
  -0.025357;0.000000;0.999678;,
  0.035988;0.000000;0.999352;,
  0.061026;0.000000;0.998136;,
  -1.000000;0.000000;0.000000;,
  0.028623;0.999590;0.000000;,
  0.028613;-0.999591;-0.000000;,
  0.117218;-0.000615;-0.993106;,
  -0.004281;-0.999991;0.000047;,
  -0.004278;0.999991;0.000047;,
  0.999940;0.000000;-0.010925;,
  0.007530;0.010503;-0.999917;,
  -0.001034;0.020060;-0.999798;,
  0.010473;-0.002230;-0.999943;,
  0.001484;-0.001654;-0.999998;,
  0.013416;-0.000000;-0.999910;,
  0.004002;-0.000000;-0.999992;,
  0.010473;0.002230;-0.999943;,
  0.001484;0.001654;-0.999998;,
  0.007529;-0.010504;-0.999917;,
  -0.001034;-0.020061;-0.999798;,
  -0.017126;0.019111;-0.999671;,
  -0.017978;0.001153;-0.999838;,
  -0.018826;-0.000000;-0.999823;,
  -0.017977;-0.001153;-0.999838;,
  -0.017125;-0.019111;-0.999671;,
  -0.680925;0.732353;0.000000;,
  0.001034;-0.020054;0.999798;,
  -0.007527;-0.010500;0.999917;,
  0.000000;0.000000;1.000000;,
  -0.952331;0.305067;0.000000;,
  -0.001483;0.001653;0.999998;,
  -0.010469;0.002229;0.999943;,
  -1.000000;0.000002;0.000000;,
  -0.004001;0.000000;0.999992;,
  -0.013412;0.000000;0.999910;,
  -0.952332;-0.305063;0.000000;,
  -0.001483;-0.001653;0.999998;,
  -0.010469;-0.002229;0.999943;,
  -0.680912;-0.732365;0.000000;,
  0.001034;0.020054;0.999798;,
  -0.007527;0.010500;0.999917;,
  -0.061609;0.000000;0.998100;,
  -0.025357;0.000000;0.999678;,
  0.035988;0.000000;0.999352;,
  0.061026;0.000000;0.998136;,
  -0.036802;-0.999262;0.011028;,
  -0.024046;-0.999574;0.016512;,
  0.030117;-0.999486;0.011007;,
  0.071464;-0.997443;-0.000000;,
  1.000000;0.000000;0.000000;,
  -0.013527;-0.999908;0.000000;,
  -0.013528;0.999908;0.000000;,
  -0.023466;0.000000;-0.999725;,
  -0.055643;-0.998451;0.000000;,
  -0.055655;0.998450;0.000000;,
  -0.999940;0.000000;0.010925;,
  0.010928;0.000000;0.999940;,
  0.017120;-0.019104;0.999671;,
  0.017972;-0.001153;0.999838;,
  0.018820;0.000000;0.999823;,
  0.017972;0.001153;0.999838;,
  0.017120;0.019105;0.999671;,
  0.680929;0.732350;0.000000;,
  0.952332;0.305064;0.000000;,
  1.000000;0.000002;0.000000;,
  0.952333;-0.305060;-0.000000;,
  0.680916;-0.732361;-0.000000;,
  0.249819;0.968293;0.000000;,
  0.000001;1.000000;0.000000;,
  -0.249821;0.968292;0.000000;,
  0.249788;-0.968301;-0.000000;,
  0.000001;-1.000000;-0.000000;,
  -0.249790;-0.968300;0.000000;;
  88;
  4;0,1,2,0;,
  4;0,2,3,0;,
  4;0,3,9,0;,
  4;1,4,4,2;,
  4;3,5,5,9;,
  4;4,6,7,4;,
  4;5,8,10,5;,
  4;11,14,14,11;,
  4;14,16,16,14;,
  4;16,19,19,16;,
  4;64,65,15,12;,
  4;12,15,15,13;,
  4;13,15,23,22;,
  4;65,66,17,15;,
  4;15,18,24,23;,
  4;66,67,20,17;,
  4;17,20,21,18;,
  4;18,21,25,24;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;70,71,71,70;,
  4;72,72,72,72;,
  4;72,72,72,72;,
  4;72,72,72,72;,
  4;26,26,26,26;,
  4;26,26,26,26;,
  4;26,26,26,26;,
  4;26,26,26,26;,
  4;28,73,28,28;,
  4;27,27,74,27;,
  4;75,75,75,75;,
  4;29,7,8,29;,
  3;76,73,76;,
  3;77,77,74;,
  4;72,72,72,72;,
  4;30,30,30,30;,
  4;32,32,32,32;,
  4;31,31,31,31;,
  4;78,78,78,78;,
  4;79,79,79,79;,
  4;0,0,0,0;,
  4;0,0,35,33;,
  4;33,35,36,34;,
  4;34,36,44,43;,
  4;0,0,0,0;,
  4;0,0,37,35;,
  4;35,37,38,36;,
  4;36,38,45,44;,
  4;0,0,0,0;,
  4;0,0,39,37;,
  4;37,39,40,38;,
  4;38,40,46,45;,
  4;0,0,0,0;,
  4;0,0,41,39;,
  4;39,41,42,40;,
  4;40,42,47,46;,
  4;48,52,52,48;,
  4;52,55,55,52;,
  4;55,58,58,55;,
  4;58,61,61,58;,
  4;80,81,53,49;,
  4;49,53,54,50;,
  4;50,54,51,51;,
  4;51,51,51,51;,
  4;81,82,56,53;,
  4;53,56,57,54;,
  4;54,57,51,51;,
  4;51,51,51,51;,
  4;82,83,59,56;,
  4;56,59,60,57;,
  4;57,60,51,51;,
  4;51,51,51,51;,
  4;83,84,62,59;,
  4;59,62,63,60;,
  4;60,63,51,51;,
  4;51,51,51,51;,
  4;85,86,86,85;,
  4;86,87,87,86;,
  4;87,88,88,87;,
  4;88,89,89,88;,
  4;85,85,90,90;,
  4;90,90,91,91;,
  4;91,91,92,92;,
  4;92,92,48,48;,
  4;89,89,93,93;,
  4;93,93,94,94;,
  4;94,94,95,95;,
  4;95,95,61,61;;
 }
}
