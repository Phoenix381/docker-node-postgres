DROP FUNCTION IF EXISTS edmonds_carp_algorithm(integer,integer);

CREATE OR REPLACE FUNCTION edmonds_carp_algorithm(source_vertex INTEGER, sink_vertex INTEGER) RETURNS interv AS $$
DECLARE
    -- temp storage
    temp_var RECORD;
    current_vertex RECORD;
    min_capacity interv;
    -- output
    max_flow interv := '0;0';
BEGIN
    -- Initialize residual graph
    DROP TABLE IF EXISTS residual_graph;
    
    CREATE TEMP TABLE residual_graph AS
    SELECT source, target, capacity, capacity AS residual_capacity 
    FROM edge_list;
    
    -- Log residual graph
    -- RAISE NOTICE 'Residual graph:';
    -- FOR temp_var IN SELECT * FROM residual_graph LOOP
    --     -- Construct the log message
    --     RAISE NOTICE 'start: %, finish: %, capacity: %, res_cap: %', temp_var.source, temp_var.target, temp_var.capacity, temp_var.residual_capacity;
    -- END LOOP;

    -- Init temp tables
    DROP TABLE IF EXISTS visited;
    DROP TABLE IF EXISTS queue;
    -- DROP TABLE IF EXISTS graph_path;

    CREATE TEMP TABLE visited (
        vertex integer,
        parent integer
    );

    CREATE TEMP TABLE queue (
        vertex integer
    );
    
    -- Main loop
    LOOP
        -- Initialize BFS
        TRUNCATE visited;
        TRUNCATE queue;

        INSERT INTO queue VALUES (source_vertex);
        INSERT INTO visited VALUES (source_vertex, 0);

        -- BFS
        LOOP
            -- Break if queue is empty
            IF NOT EXISTS (SELECT 1 FROM queue) THEN
                EXIT;
            END IF;

            -- Select next node
            SELECT * INTO current_vertex FROM queue LIMIT 1;
            DELETE FROM queue WHERE vertex = current_vertex.vertex;
            -- Add adjacent nodes to queue
            INSERT INTO queue
            SELECT target
            FROM residual_graph
            WHERE source = current_vertex.vertex
                AND residual_capacity > '0;0'
                AND target NOT IN (SELECT vertex FROM visited);

            -- RAISE NOTICE 'Processing: %', (SELECT STRING_AGG(vertex::TEXT, ' , ') FROM queue);
            -- Mark parent for each vertex
            INSERT INTO visited
            SELECT target, current_vertex.vertex
            FROM residual_graph
            WHERE source = current_vertex.vertex
                AND residual_capacity > '0;0'
                AND target NOT IN (SELECT vertex FROM visited);

            -- RAISE NOTICE 'Visited: %', (SELECT STRING_AGG(vertex::TEXT, ' , ') FROM visited);

            -- Break if node is sink
            IF current_vertex.vertex = sink_vertex THEN
                EXIT;
            END IF;
        END LOOP;

        -- Reconstruct path from visited
        DROP TABLE IF EXISTS graph_path;
        CREATE TEMP TABLE graph_path (
            source integer,
            target integer
        );

        SELECT * INTO current_vertex 
        FROM visited 
        WHERE vertex = sink_vertex
        LIMIT 1;

        INSERT INTO graph_path VALUES (current_vertex.parent, current_vertex.vertex);

        LOOP
            SELECT * INTO current_vertex FROM visited
            WHERE vertex = current_vertex.parent
            LIMIT 1;

            INSERT INTO graph_path VALUES (current_vertex.parent, current_vertex.vertex);

            IF current_vertex.parent = 0 OR current_vertex.parent is NULL THEN
                EXIT;
            END IF;
        END LOOP;

        IF current_vertex.parent is NULL THEN
            -- RAISE NOTICE 'finishing...';
            EXIT;
        END IF;

        -- Break if path is empty
        IF NOT EXISTS (SELECT 1 FROM graph_path) THEN
            EXIT;
        END IF;

        -- Display path
        -- RAISE NOTICE 'Path found: %', (SELECT STRING_AGG(target::TEXT, ' <- ') FROM graph_path);

        -- Determine bottleneck checking each edge
        SELECT interv_min(residual_capacity) INTO min_capacity
        FROM residual_graph rg
        JOIN graph_path p
        ON rg.source=p.source AND rg.target=p.target;

        -- RAISE NOTICE 'Bottleneck: %', min_capacity;

        -- Update residual graph
        UPDATE residual_graph
        SET residual_capacity = residual_capacity - min_capacity
        FROM graph_path
        WHERE residual_graph.source = graph_path.source
          AND residual_graph.target = graph_path.target;

        -- Update flow
        max_flow := max_flow + min_capacity;

        -- RAISE NOTICE 'Max Flow: %', max_flow;
    END LOOP;

    -- Output the maximum flow
    -- RAISE NOTICE 'Result Max Flow: %', max_flow;
    RETURN max_flow;
END;
$$ LANGUAGE plpgsql;


SELECT * from edmonds_carp_algorithm(1,9);