type internalErrorMessage = string;

type externalErrorMessage = string;

exception GraphQLException(internalErrorMessage, externalErrorMessage);

let failPublic_ = (internalErrorMessage, externalErrorMessage) =>
  raise(GraphQLException(internalErrorMessage, externalErrorMessage));

let failPublic = (~internal=?, ~public, ()) =>
  failPublic_(
    switch (internal) {
    | None => public
    | Some(internal) => internal
    },
    public,
  );

let raiseErr = message => raise(Failure(message));
