let fromHex = s => Cryptokit.transform_string(Cryptokit.Hexa.decode(), s);

let toHex = s => Cryptokit.transform_string(Cryptokit.Hexa.encode(), s);

let aesEncryptString = (key, plainStr) => {
  let encoder =
    Cryptokit.Cipher.aes(
      ~pad=Cryptokit.Padding.length,
      key,
      Cryptokit.Cipher.Encrypt,
    );
  encoder#put_string(plainStr);
  encoder#finish;
  encoder#get_string;
};

let aesDecryptString = (key, encryptedStr) => {
  let decoder =
    Cryptokit.Cipher.aes(
      ~pad=Cryptokit.Padding.length,
      key,
      Cryptokit.Cipher.Decrypt,
    );
  decoder#put_string(encryptedStr);
  decoder#finish;
  decoder#get_string;
};

module EncryptedString = {
  type t = {_encryptedValue: string};
  let ofEncryptedString = encryptedStr => {_encryptedValue: encryptedStr};
  let encryptString = (key, plainStr) : t => {
    _encryptedValue: toHex(aesEncryptString(key, plainStr)),
  };
  let encryptedString = encryptedStr : string => encryptedStr._encryptedValue;
  let decryptedString = (key, encryptedStr: t) : string =>
    aesDecryptString(key, fromHex(encryptedStr._encryptedValue));
};

module EncryptedJSON = {
  type t = EncryptedString.t;
  let ofEncryptedString = EncryptedString.ofEncryptedString;
  let encryptJSON = (key, json: Yojson.Basic.json) : t =>
    EncryptedString.encryptString(key, Yojson.Basic.to_string(json));
  let encryptString = EncryptedString.encryptString;
  let encryptedJSON = EncryptedString.encryptedString;
  let decryptedJSON = (key, encryptedStr: t) : Yojson.Basic.json =>
    Yojson.Basic.from_string(
      EncryptedString.decryptedString(key, encryptedStr),
    );
  let decryptedString = EncryptedString.decryptedString;
};
