{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE TypeApplications #-}
{-# LANGUAGE ScopedTypeVariables #-}

module Interpreter where

import qualified Data.Map                      as Map
import           DataStructures
import           Control.Monad.Except
import           Control.Monad.State
import           Control.Error                            ( fromMaybe )
import           Control.Monad.Reader
import           Pretty
import           Data.Data

class Typeable c => Extractable c where
    extract :: (MonadError StacklangError m) => Expr -> m c
    error' :: (MonadError StacklangError m) => Expr -> m c
    error' e = throwError $ MiscError ("expected " <> show (typeRep (Proxy @c)) <> " but got " <> showP e)

instance Extractable Int where
  extract (Atom (Number n)) = return n
  extract e                 = error' e

instance Extractable [Expr] where
  extract (List l) = return l
  extract e        = error' e

instance Extractable String where
  extract (Atom (Declaration a)) = return a
  extract e                      = error' e

instance Extractable Expr where
  extract = return

pop :: (Extractable a, MonadError StacklangError m, MonadState Stack m) => m a
pop = do
  s <- get
  case s of
    (e : xs) -> put xs >> extract e
    []       -> throwError $ StackTooSmall []

top :: (Extractable a, MonadError StacklangError m, MonadState Stack m) => m a
top = do
  s <- get
  case s of
    (e : _) -> extract e
    []      -> throwError $ StackTooSmall []

push :: (MonadState Stack m) => Expr -> m ()
push x = modify (x :)

rethrowTypeErr :: (MonadError StacklangError m) => String -> StacklangError -> m a
rethrowTypeErr name e = throwError $ case e of
  (TypeError _ x) -> TypeError name x
  v               -> v

binMathOp :: (MonadError StacklangError m, MonadState Stack m) => (Int -> Int -> Int) -> m ()
binMathOp op = do
  x_i <- pop
  y_i <- pop
  push (Atom . Number $ op x_i y_i)

liftBool :: (Int -> Int -> Bool) -> Int -> Int -> Int
liftBool f x y = if f x y then 1 else 0

dup :: (MonadError StacklangError m, MonadState Stack m) => m ()
dup = do
  a <- top
  push a

ifThenElse :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => m ()
ifThenElse = do
  _else <- pop
  _if   <- pop
  cond  <- pop @Int
  evalFunc (if cond /= 0 then _if else _else)

ifThen :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => m ()
ifThen = do
  _if  <- pop
  cond <- pop @Int
  when (cond /= 0) $ evalFunc _if

exec :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => m ()
exec = do
  f <- pop
  evalFunc f

defineIn :: (MonadError StacklangError m, MonadState Stack m, MonadReader Variables m) => m () -> m ()
defineIn env = do
  x    <- pop @Expr
  decl <- pop @String
  local (Map.insert decl x) env

printerLn :: (MonadError StacklangError m, MonadIO m, MonadState Stack m) => m ()
printerLn = do
  printer
  liftIO $ putStrLn ""

printer :: (MonadError StacklangError m, MonadIO m, MonadState Stack m) => m ()
printer = do
  x <- pop @Expr
  liftIO . putStr $ case x of
    (Atom (String s)) -> s
    _                 -> showP x

swap :: (MonadError StacklangError m, MonadState Stack m) => m ()
swap = do
  a <- pop @Expr
  b <- pop @Expr
  push a
  push b

stdlib :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => Map.Map String (m ())
stdlib = Map.fromList
  [ ("+"     , binMathOp (+))
  , ("*"     , binMathOp (*))
  , ("-"     , binMathOp . flip $ (-))
  , ("div"   , binMathOp . flip $ div)
  , ("%"     , binMathOp . flip $ mod)
  , (">"     , binMathOp . liftBool . flip $ (>))
  , ("<"     , binMathOp . liftBool . flip $ (<))
  , (">="    , binMathOp . liftBool . flip $ (>=))
  , ("<="    , binMathOp . liftBool . flip $ (<=))
  , ("=="    , binMathOp . liftBool $ (==))
  , ("!="    , binMathOp . liftBool $ (/=))
  , ("eq"    , binMathOp . liftBool $ (==))
  , ("neq"   , binMathOp . liftBool $ (/=))
  , ("geq"   , binMathOp . liftBool . flip $ (>))
  , ("leq"   , binMathOp . liftBool . flip $ (<))
  , ("ifelse", ifThenElse)
  , ("?"     , ifThenElse)
  , ("if"    , ifThen)
  , ("exec"  , exec)
  , ("print" , printerLn)
  , ("output", printer)
  , ("dup"   , dup)
  , ("pop"   , void (pop @Expr))
  , ("swap"  , swap)
  ]

evalFunc :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => [Expr] -> m ()
evalFunc []       = return ()
evalFunc (x : xs) = case x of
  (Atom (Identifier "="  )) -> defineIn (evalFunc xs)
  (Atom (Identifier "def")) -> defineIn (evalFunc xs)
  _                         -> eval x >> evalFunc xs

evalStdlib :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => String -> m ()
evalStdlib fS = fromMaybe (throwError $ NoSuchFunction fS) (Map.lookup fS stdlib)

eval :: (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m) => Expr -> m ()
eval (Atom (Identifier s)) = do
  v <- ask
  case Map.lookup s v of
    Just x -> case x of
      (List l) -> evalFunc l
      _        -> eval x
    Nothing -> evalStdlib s
eval a = push a
