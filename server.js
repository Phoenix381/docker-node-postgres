
const path = require('path');
const express = require('express');
const { Pool } = require('pg');

const app = express();
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

// Define a simple PL/pgSQL function
const createPlpgsqlFunction = `
CREATE OR REPLACE FUNCTION get_example_data()
RETURNS TABLE(id INT, name TEXT) AS $$
BEGIN
    RETURN QUERY SELECT 1 AS id, 'Example' AS name;
END;
$$ LANGUAGE plpgsql;
`;

// Execute the PL/pgSQL function
pool.query(createPlpgsqlFunction, (err, res) => {
  if (err) {
    console.error('Error creating PL/pgSQL function:', err.stack);
  } else {
    console.log('PL/pgSQL function created successfully');
  }
});

// Serve the index.html file by default
app.use(express.static(path.join(__dirname, 'public')));

// Define a route to fetch data using the PL/pgSQL function
app.get('/api/data', async (req, res) => {
  try {
    const result = await pool.query('SELECT * FROM get_example_data()');
    res.json(result.rows);
  } catch (err) {
    console.error('Error executing PL/pgSQL function:', err.stack);
    res.status(500).send('Internal Server Error');
  }
});

app.listen(port, () => {
  console.log(`Web server running at http://localhost:${port}`);
});