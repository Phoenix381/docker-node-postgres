
const path = require('path');
const express = require('express');
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

// Servin from public folder
app.use(express.static(path.join(__dirname, 'public')));

// api post request
app.post('/api/run', async (req, res) => {
    try {
        // Check the payload
        const payload = req.body;

        // Log the payload for debugging
        console.log('Payload:', payload);

        // Perform any necessary operations with the payload
        // For example, you can validate the payload here
        if (!payload) {
            return res.status(400).json({ error: 'Invalid payload' });
        }

        // Respond with a success message
        res.json({ message: 'Payload received successfully', payload });
    } catch (error) {
        console.error('Error processing payload:', error);
        res.status(500).json({ error: 'Internal Server Error' });
    }
});


app.listen(port, () => {
  console.log(`Web server running at http://localhost:${port}`);
});