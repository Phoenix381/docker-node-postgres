FROM node:14

WORKDIR /usr/src/app

# COPY package*.json ./
COPY . .

RUN npm install

# Run the web server
CMD ["node", "server.js"]