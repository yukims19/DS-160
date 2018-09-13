open Async;

let blockHash = (~count=6, stringToHash) =>
  Bcrypt.hash(~count, stringToHash) |> Bcrypt.string_of_hash;

let nonBlockHash = (~count=6, stringToHash) =>
  In_thread.run(() =>
    Bcrypt.hash(~count, stringToHash) |> Bcrypt.string_of_hash
  );
