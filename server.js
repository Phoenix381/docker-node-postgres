
const path = require('path');
const express = require('express');
const fs = require('fs');
const { Pool } = require('pg');

const app = express();

// Middleware to parse JSON and URL-encoded data
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// !!! server port
const port = 3000;

// Create a PostgreSQL client pool
const pool = new Pool({
  user: process.env.DB_USER,
  host: process.env.DB_HOST,
  database: process.env.DB_NAME,
  password: process.env.DB_PASSWORD,
  port: 5432,
});


// Execute the PL/pgSQL function
async function ec_flow(data, targets) {
  let query = 'TRUNCATE TABLE edge_list;\n';

  // Build the query to populate edge_list
  for (let row of data) {
    query += `INSERT INTO edge_list (source, target, capacity) VALUES ('${row.source}', '${row.target}', '${row.capacity}');\n`;
  }

  // Execute the truncate and insert queries
  try {
    await pool.query(query);
    console.log('edge_list populated...');
  } catch (err) {
    console.error('Error populating edge_list:', err.stack);
    throw err; // Propagate the error to the caller
  }

  // Execute the Edmonds-Karp algorithm query
  const es_query = `SELECT * from edmonds_carp_algorithm('${targets.source}', '${targets.target}');`;
  try {
    const result = await pool.query(es_query);
    // console.log('result is:', result.rows);
    return result.rows;
  } catch (err) {
    console.error('Error executing PL/pgSQL function:', err.stack);
    throw err; // Propagate the error to the caller
  }
}

// Serving from public folder
app.use(express.static(path.join(__dirname, 'public')));

// api post request
app.post('/api/run', async (req, res) => {
  try {
    const payload = req.body;

    if (!payload) {
      return res.status(400).json({ error: 'Invalid payload' });
    }

    // Call ec_flow and wait for it to complete
    const result = await ec_flow(payload.graph, payload.targets);
    console.log('Result:', result);

    // Respond with the result
    res.json(result);
  } catch (error) {
    console.error('Error processing payload:', error);
    res.status(500).json({ error: 'Internal Server Error' });
  }
});


app.listen(port, () => {
  console.log(`Web server running at http://localhost:${port}`);
});