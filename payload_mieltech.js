function decodeUplink(input) {
    // On attend 20 octets : 8 *int16/uint16* = 16 octets + 1 float (4 octets)
    if (input.bytes.length < 20) {
      return {
        errors: ["Payload trop court : 20 octets attendus."]
      };
    }
  
    // Lecture int16 en little-endian (temp, hum, poids, etc.)
    function readInt16LE(offset) {
      let val = (input.bytes[offset]) | (input.bytes[offset + 1] << 8);
      if (val & 0x8000) {
        val = val - 0x10000; // conversion négatif
      }
      return val;
    }
  
    // Lecture uint16 (non signé) pour la luminosité
    function readUInt16LE(offset) {
      return (input.bytes[offset]) | (input.bytes[offset + 1] << 8);
    }
  
    // Lecture float32 en little-endian (batterie)
    function readFloatLE(offset) {
      let bytes = input.bytes.slice(offset, offset + 4);
      let buf = new ArrayBuffer(4);
      let view = new DataView(buf);
      view.setUint8(0, bytes[0]);
      view.setUint8(1, bytes[1]);
      view.setUint8(2, bytes[2]);
      view.setUint8(3, bytes[3]);
      return view.getFloat32(0, true);
    }
  
    // --- Lecture des données brutes ---
    let t_raw      = readInt16LE(0);   // °C ×10
    let h_raw      = readInt16LE(2);   // % ×10
    let t_i_raw    = readInt16LE(4);   // °C ×10
    let h_i_raw    = readInt16LE(6);   // % ×10
    let t_0_raw    = readInt16LE(8);   // DS18 #1 ×10
    let t_1_raw    = readInt16LE(10);  // DS18 #2 ×10
    let weight_raw = readInt16LE(12);  // kg ×100
    let lux_raw    = readUInt16LE(14); // luminosité (uint16)
  
    let rawBattery = readFloatLE(16);  // batterie (float32)
  
    // --- Conversion vers unités réelles ---
    let t         = t_raw / 10.0;
    let h         = h_raw / 10.0;
    let t_i       = t_i_raw / 10.0;
    let h_i       = h_i_raw / 10.0;
    let t_0       = t_0_raw / 10.0;
    let t_1       = t_1_raw / 10.0;
    let weight_kg = weight_raw / 100.0;
    let bv        = parseFloat(rawBattery.toFixed(1));
  
    // --- Payload final pour BEEP ---
    return {
      data: {
        key: "9t5zkkdsdbfvb2ej",
  
        t: t,
        h: h,
        t_i: t_i,
        h_i: h_i,
        t_0: t_0,
        t_1: t_1,
        weight_kg: weight_kg,
        bv: bv,
        l: lux_raw
      }
    };
  }
  
