# InvertedIndex

# To run the following programn, the VS studio is needed. 
## Visual Studio uses project files with extensions like .vcxproj for C++ projects instead of traditional Makefiles. To run the program you need the C++ 17.


# To run the python code:
# API Invert Index
# Getting Started
**Clone the project from GitHub**:
   ```bash
   git clone https://github.com/alexandrashy/course_work_parallel_computing.git
   ```
Before starting, install [python 3.9.10](https://www.python.org/downloads/release/python-3910/) as the main version  
To run the project, follow these steps:

1. **Create a virtual environment (optional but recommended)**:

    ```bash
    python -m venv venv
    ```

    - **Activate the virtual environment**:

      - For Windows:

        ```bash
        .\venv\Scripts\activate
        ```

      - For Unix or MacOS:

        ```bash
        source venv/bin/activate
        ```

2. **Install project dependencies**:

    ```bash
    pip install -r requirements.txt
    ```
   
3. **Run the project**:

    ```bash
    python Client.py
    ```

    If using a virtual environment, make sure it's activated before running the project.

4. **Deactivate the virtual environment (if used)**:

    ```bash
    deactivate
    ```

The API server will start locally at `http://localhost:5000/`.

## API Endpoints

### Put the task into execution
Endpoint: `http://localhost:5000/SendPathtoExecute`  
Method: `POST`

Response:
```json
{
  "message": "Task added to execution"
}
```

### Check if the InvertedIndex has been created
Endpoint: `http://localhost:5000/GetExecutedData`  
Method: `GET`  
Response:
```json
{
  "message": "File proccessed"
}
```

