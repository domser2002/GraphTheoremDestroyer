from enum import Enum

class ResponseResult(Enum):
    SUCCESS = 1
    FAILURE = 2

class Response:
    '''
    model that contains response data
    from backend
    '''
    def __init__(self, result: ResponseResult, proof_path: str):
        self.result:ResponseResult = result
        self.proof_path:str = proof_path
        if self.result == ResponseResult.FAILURE:
            self.proof_path = ''