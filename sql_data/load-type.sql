
DROP TYPE IF EXISTS interv CASCADE;

CREATE FUNCTION interv_in(cstring)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION interv_out(interv)
   RETURNS cstring
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION interv_recv(internal)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION interv_send(interv)
   RETURNS bytea
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   

CREATE TYPE interv(
   internallength = 16,
   input = interv_in,
   output = interv_out,
   receive = interv_recv,
   send = interv_send,
   alignment = double
);

-- FUNCTIONS
CREATE FUNCTION mid(interv)
   RETURNS real
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION rad(interv)
   RETURNS real
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION width(interv)
   RETURNS real
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION down(interv)
   RETURNS real
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION up(interv)
   RETURNS real
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
-- TESTS


CREATE FUNCTION scalar_mult(interv, real)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;

-- ISUM
CREATE FUNCTION interv_add(interv, interv)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE AGGREGATE interv_sum (
   sfunc = interv_add,
   basetype = interv,
   stype = interv,
   initcond = '0;0'
);


CREATE OPERATOR + (
   leftarg = interv, 
   rightarg = interv, 
   procedure = interv_add
);

-- ISUB
CREATE FUNCTION interv_subst(interv, interv)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE AGGREGATE interv_sub (
   sfunc = interv_subst,
   basetype = interv,
   stype = interv,
   initcond = '0;0'
);

CREATE OPERATOR - (
   leftarg = interv, 
   rightarg = interv, 
   procedure = interv_subst
);

-- IMUL
CREATE FUNCTION interv_mult(interv, interv)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;
   
CREATE FUNCTION interv_scalar_mult(interv, double precision)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE interv_mul (
   sfunc = interv_mult,
   basetype = interv,
   stype = interv,
   initcond = '1;1'
);

CREATE OPERATOR * (
   leftarg = interv, 
   rightarg = double precision, 
   procedure = interv_scalar_mult
);

-- ICOUNT
-- TODO ----------------------------------

-- AGGREGATE TESTS


-- OPERATORS
-- <=
CREATE FUNCTION interv_abs_le(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   leftarg = interv, rightarg = interv, procedure = interv_abs_le,
   commutator = >= , negator = > ,
   restrict = scalarlesel, join = scalarlejoinsel
);

-- >=
CREATE FUNCTION interv_abs_ge(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   leftarg = interv, rightarg = interv, procedure = interv_abs_ge,
   commutator = <= , negator = < ,
   restrict = scalargesel, join = scalargejoinsel
);

-- >
CREATE FUNCTION interv_abs_gt(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   leftarg = interv, rightarg = interv, procedure = interv_abs_gt,
   commutator = < , negator = <= ,
   restrict = scalargesel, join = scalargejoinsel
);

-- <
CREATE FUNCTION interv_abs_lt(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = interv, rightarg = interv, procedure = interv_abs_lt,
   commutator = > , negator = >= ,
   restrict = scalargesel, join = scalargejoinsel
);


-- =
CREATE FUNCTION interv_abs_eq(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   leftarg = interv, rightarg = interv, procedure = interv_abs_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);

-- <>
CREATE FUNCTION interv_abs_neq(interv, interv) RETURNS bool
   AS '/var/lib/postgresql/scripts/complex'  LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   leftarg = interv, rightarg = interv, procedure = interv_abs_neq,
   commutator = = ,
   negator = <> ,
   restrict = neqsel, join = neqjoinsel
);

-- MIN aggregate
CREATE FUNCTION interv_min_func(interv, interv)
   RETURNS interv
   AS '/var/lib/postgresql/scripts/complex' 
   LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE interv_min (
   sfunc = interv_min_func,
   basetype = interv,
   stype = interv,
   initcond = '1000000;1000000'
);

CREATE FUNCTION interv_array_equal(interv[], interv[]) 
	RETURNS bool 
	AS '/var/lib/postgresql/scripts/complex'
	LANGUAGE C IMMUTABLE STRICT;

-- Create the operator class for the equality operator
CREATE OPERATOR = (
   leftarg = interv[], rightarg = interv[], procedure = interv_array_equal,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);

-- INIT graph table
DROP TABLE IF EXISTS edge_list;

CREATE TABLE edge_list (
    source int, 
    target int, 
    capacity interv
);