#include "bits/stdc++.h"


extern "C" {
#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"	
#include "utils/array.h"

    PG_MODULE_MAGIC;



    // typedef struct Interval {
    // 	double		x;
    // 	double		y;
    // }			Interval;


    class Interval {
    public:
        double down, up;
        double p = 0.9;
        Interval(double _down = 0.0, double _up = 0.0) : down(_down), up(_up) {}

        Interval& operator+(Interval& other) {
            Interval interv(down + other.down, up + other.up);
            return interv;
        }
        Interval& operator-(Interval& other) {
            Interval interv(
                std::max(down - other.down, 0.0),
                std::max(up - other.up, 0.0)
            );
            return interv;
        }

        Interval& operator*(Interval& other) {
            Interval interv(down * other.down, up * other.up);
            return interv;
        }
        Interval& operator*(int other) {
            Interval interv(down * other, up * other);
            return interv;
        }


        Interval& operator=(Interval& other) {
            if (this != &other) {
                down = other.down;
                up = other.up;
            }
            return *this;
        }
        double mid(const Interval& x)
        {
            return (x.down + x.up) / 2.0;
        }
        double rad(const Interval& x)
        {
            return (x.up - x.down) / 2.0;
        }
        char* to_string() {
            char output[50];
            //char test[12] = "test";
            snprintf(output, 50, "%.2f;%.2f", down, up);
            return output;
        }
        bool operator ==(Interval& obj2) {
            if (down == obj2.down && up == obj2.up)
                return true;
            else
                return false;
        }
        bool operator !=(Interval& obj2) {
            if (down == obj2.down && up == obj2.up)
                return false;
            else
                return true;
        }

        bool operator <=(Interval& obj2) {
            if (down == obj2.down && up == obj2.up)
                return true;

            // скаляры
            if(up == down && obj2.up < up)
                return false;
            else if(up == down && obj2.down > up)
                return true;
            else if(up == down && (obj2.up + obj2.down)/2 > up)
                return true;
            else if(up == down && (obj2.up + obj2.down)/2 < up)
                return false;

            if(obj2.up == obj2.down && up < obj2.up)
                return true;
            else if(obj2.up == obj2.down && down > obj2.up)
                return false;
            else if(obj2.up == obj2.down && (up + down)/2 > obj2.up)
                return false;
            else if(obj2.up == obj2.down && (up + down)/2 < obj2.up)
                return true;



            // double r_x = rad(*this);
            // double r_y = rad(obj2);

            // double R = (mid(obj2) - mid(*this)) / (r_x + r_y);
            // double P = 0;
            // if (R < ((-1) * abs(r_x - r_y)) / (r_x + r_y))
            //     P = ((r_x + r_y) * (r_x + r_y) * (1 + R) * (1 + R)) / (8.0 * r_x * r_y);
            // if (abs(R) <= (abs(r_x - r_y)) / (r_x + r_y))
            //     P = 0.5 + ((r_x + r_y) * R) / (2.0 * r_y);
            // if (R > (abs(r_x - r_y)) / (r_x + r_y))
            //     P = 1.0 - ((r_x + r_y) * (r_x + r_y) * (1 - R) * (1 - R)) / (8.0 * r_x * r_y);


            // return P < p;

            // // пересечение
            // if( (up-down) * (up-obj2.down)+(down-obj2.down) < (up-down) * (obj2.up-up)+(obj2.up-down) )
            //     return true;
            // if(  )

            // граничные условия
            if(down < obj2.down && up < obj2.up)
                return true;
            if(down > obj2.down && up > obj2.up)
                return false;


            // 1<2
            // 1
            if( down == obj2.up && obj2.up > up   )
                return true;
            if( ((obj2.up - down)*(obj2.up - down))/2  >  (up-down)*(obj2.up-obj2.down) - ((obj2.up - down)*(obj2.up - down))/2 )
                return true;
            // 2 трапеции
            if( (up-down) * (up-obj2.down)+(down-obj2.down) < (up-down) * (obj2.up-up)+(obj2.up-down) )
                return true;

            

            
            return false; 
        }

        bool operator <(Interval& obj2) {
            if (*this <= obj2 && *this != obj2)
                return false;
            else
                return true;
        }
        bool operator >(Interval& obj2) {
            if (obj2 <= *this && *this != obj2)
                return false;
            else
                return true;
        }
        bool operator >=(Interval& obj2) {
            if (obj2 <= *this)
                return true;
            else
                return false;
        }

        // as funcs

        bool interv_le(Interval* other) {
            Interval obj2 = *other;

            if (down == obj2.down && up == obj2.up)
                return true;

            double r_x = rad(*this);
            double r_y = rad(obj2);

            double R = (mid(obj2) - mid(*this)) / (r_x + r_y);
            double P = 0;
            if (R < ((-1) * abs(r_x - r_y)) / (r_x + r_y))
                P = ((r_x + r_y) * (r_x + r_y) * (1 + R) * (1 + R)) / (8.0 * r_x * r_y);
            if (abs(R) <= (abs(r_x - r_y)) / (r_x + r_y))
                P = 0.5 + ((r_x + r_y) * R) / (2.0 * r_y);
            if (R > (abs(r_x - r_y)) / (r_x + r_y))
                P = 1.0 - ((r_x + r_y) * (r_x + r_y) * (1 - R) * (1 - R)) / (8.0 * r_x * r_y);


            return P < p;
        }

        bool interv_eq(Interval* obj2) {
            if (down == obj2->down && up == obj2->up)
                return true;
            else
                return false;
        }

        bool interv_neq(Interval* obj2) {
            if (down == obj2->down && up == obj2->up)
                return false;
            else
                return true;
        }
        
        bool interv_lt(Interval* other) {
            return interv_le(other) && ! interv_eq(other);
        }

        bool interv_gt(Interval* other) {
            return other->interv_le(this) && ! interv_eq(other);
        }

        bool interv_ge(Interval* other) {
            return other->interv_le(this);
        }
    };




    // interval_in(cstring) - из строки делаем объект
    PG_FUNCTION_INFO_V1(interv_in);

    Datum
        interv_in(PG_FUNCTION_ARGS) {
        char* str = PG_GETARG_CSTRING(0);
        double down, up;

        if (sscanf(str, "%lf;%lf", &down, &up) != 2)
            ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for type %s: \"%s\"",
                        "complex", str)));

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(down, up);

        PG_RETURN_POINTER(result);

    }

    // interval_out(interval) - из объекта строку
    PG_FUNCTION_INFO_V1(interv_out);

    Datum
        interv_out(PG_FUNCTION_ARGS) {
        Interval* interval = (Interval*)PG_GETARG_POINTER(0);

        //auto result = interval->to_string();

        char* result;
        result = psprintf("%.5g;%.5g", interval->down, interval->up);

        PG_RETURN_CSTRING(result);
    }

    // interval_recv(internal) - из байтов структуру
    PG_FUNCTION_INFO_V1(interv_recv);

    Datum
        interv_recv(PG_FUNCTION_ARGS) {
        StringInfo	buf = (StringInfo)PG_GETARG_POINTER(0);

        double down, up;
        down = pq_getmsgfloat8(buf);
        up = pq_getmsgfloat8(buf);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(down, up);

        PG_RETURN_POINTER(result);
    }

    // interval_send(complex) - из объекта байты
    PG_FUNCTION_INFO_V1(interv_send);

    Datum
        interv_send(PG_FUNCTION_ARGS) {
        Interval* interval = (Interval*)PG_GETARG_POINTER(0);
        StringInfoData buf;

        pq_begintypsend(&buf);
        pq_sendfloat8(&buf, interval->down);
        pq_sendfloat8(&buf, interval->up);
        PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
    }
    // функции
    PG_FUNCTION_INFO_V1(mid);

    Datum
        mid(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        //PG_RETURN_FLOAT4(complex_mid_cmp_internal(x->down + x->up) / 2.0));
        PG_RETURN_FLOAT4(1.);
    }

    PG_FUNCTION_INFO_V1(rad);

    Datum
        rad(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        //PG_RETURN_FLOAT4(complex_mid_cmp_internal(x->down + x->up) / 2.0));
        PG_RETURN_FLOAT4(1.);
    }

    PG_FUNCTION_INFO_V1(width);

    Datum
        width(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        PG_RETURN_FLOAT4(1.);
    }

    PG_FUNCTION_INFO_V1(up);

    Datum
        up(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        PG_RETURN_FLOAT4(interv->up);
    }

    PG_FUNCTION_INFO_V1(down);

    Datum
        down(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        PG_RETURN_FLOAT4(interv->down);
    }

    PG_FUNCTION_INFO_V1(scalar_mult);

    Datum
        scalar_mult(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        double scalar = PG_GETARG_FLOAT8(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(interv->down * scalar, interv->up * scalar);

        PG_RETURN_POINTER(result);
    }

    PG_FUNCTION_INFO_V1(less_or_equal);

    Datum
        less_or_equal(PG_FUNCTION_ARGS) {
        Interval* interv = (Interval*)PG_GETARG_POINTER(0);
        double scalar = PG_GETARG_FLOAT8(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(interv->down * scalar, interv->up * scalar);

        PG_RETURN_POINTER(result);
    }
    // для аггрегатов
    PG_FUNCTION_INFO_V1(interv_add);

    Datum
        interv_add(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(a->down + b->down, a->up + b->up);

        // result = &(*a + *b);

        PG_RETURN_POINTER(result);
    }

    PG_FUNCTION_INFO_V1(interv_subst);

    Datum
        interv_subst(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(0.0, 0.0);

        // result = &(*a - *b);
        result->down = a->down - b->up;
        result->up = a->up - b->down;

        PG_RETURN_POINTER(result);
    }

    PG_FUNCTION_INFO_V1(interv_mult);

    Datum
        interv_mult(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(0.0, 0.0);

        // result = &(*a * *b);
        result->down = a->down * b->down;
        result->up = a->up * b->up;

        PG_RETURN_POINTER(result);
    }

    PG_FUNCTION_INFO_V1(interv_scalar_mult);

    Datum
        interv_scalar_mult(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        double* b = (double*)PG_GETARG_POINTER(1);

        Interval* result = (Interval*)palloc(sizeof(Interval));
        new(result) Interval(0.0, 0.0);

        result->down = a->down * (*b);
        result->up = a->up * (*b);

        PG_RETURN_POINTER(result);
    }

    // min
    PG_FUNCTION_INFO_V1(interv_min_func);

    Datum
        interv_min_func(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        // ereport(NOTICE,
        //     (errmsg("interv_min_func: %.2f;%.2f <= %.2f;%.2f, result: %d", a->down, a->up, b->down, b->up, *a <= *b))
        // );

        if(*a <= *b) {
            Interval* result = (Interval*)palloc(sizeof(Interval));
            new(result) Interval(a->down, a->up);
            PG_RETURN_POINTER(result);
        }
        else {
            Interval* result = (Interval*)palloc(sizeof(Interval));
            new(result) Interval(b->down, b->up);
            PG_RETURN_POINTER(result);
            // PG_RETURN_POINTER(b);
        }
    }

    // Define the equality function for arrays of interv data type
    PG_FUNCTION_INFO_V1(interv_array_equal);

    Datum interv_array_equal(PG_FUNCTION_ARGS) {
        ArrayType *arr1 = PG_GETARG_ARRAYTYPE_P(0);
        ArrayType *arr2 = PG_GETARG_ARRAYTYPE_P(1);
        int num_elems1, num_elems2;
        Interval *interv1, *interv2;
        bool result = true;

        // Check if arrays have the same number of elements
        num_elems1 = ARR_DIMS(arr1)[0];
        num_elems2 = ARR_DIMS(arr2)[0];
        if (num_elems1 != num_elems2)
            PG_RETURN_BOOL(false);

        // Iterate through each element of the arrays and compare
        for (int i = 0; i < num_elems1; i++) {
            interv1 = (Interval *) ARR_DATA_PTR(arr1) + i;
            interv2 = (Interval *) ARR_DATA_PTR(arr2) + i;
            if (interv1->down != interv2->down || interv1->up != interv2->up) {
                result = false;
                break;
            }
        }

        PG_RETURN_BOOL(result);
    }
    // операции
    PG_FUNCTION_INFO_V1(interv_abs_le);

    Datum
        interv_abs_le(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        // ereport(NOTICE,
        //     (errmsg("interv_abs_le: %.2f;%.2f <= %.2f;%.2f, result: %d", a->down, a->up, b->down, b->up, *a <= *b))
        // );
        PG_RETURN_BOOL(a->interv_le(b));
        // PG_RETURN_BOOL(*a <= *b);
    }

    PG_FUNCTION_INFO_V1(interv_abs_lt);

    Datum
        interv_abs_lt(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        PG_RETURN_BOOL(a->interv_lt(b));
        // PG_RETURN_BOOL(a < b);
    }

    PG_FUNCTION_INFO_V1(interv_abs_ge);

    Datum
        interv_abs_ge(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        // ereport(NOTICE,
        //     (errmsg("interv_abs_le: %.2f;%.2f <= %.2f;%.2f, result: %d", a->down, a->up, b->down, b->up, *a >= *b))
        // );
        PG_RETURN_BOOL(a->interv_ge(b));
        // PG_RETURN_BOOL(*a >= *b);
    }

    PG_FUNCTION_INFO_V1(interv_abs_gt);

    Datum
        interv_abs_gt(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        // ereport(NOTICE,
        //     (errmsg("interv_abs_le: %.2f;%.2f <= %.2f;%.2f, result: %d", a->down, a->up, b->down, b->up, *a >= *b))
        // );

        PG_RETURN_BOOL(a->interv_gt(b));
        // PG_RETURN_BOOL(*a > *b);
    }


    PG_FUNCTION_INFO_V1(interv_abs_eq);

    Datum
        interv_abs_eq(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        PG_RETURN_BOOL(a->interv_eq(b));
        // PG_RETURN_BOOL(*a == *b);
    }

    PG_FUNCTION_INFO_V1(interv_abs_neq);

    Datum
        interv_abs_neq(PG_FUNCTION_ARGS) {
        Interval* a = (Interval*)PG_GETARG_POINTER(0);
        Interval* b = (Interval*)PG_GETARG_POINTER(1);

        PG_RETURN_BOOL(a->interv_neq(b));
        // PG_RETURN_BOOL(*a != *b);
    }


}