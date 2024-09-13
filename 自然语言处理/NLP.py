import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
# 加载乳腺癌数据集
data = load_breast_cancer()
X, y = data.data, data.target


# 标准化特征
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)
# 将标签转换为列向量
y = y.reshape(-1, 1)


# 划分训练集、验证集和测试集
# 先划分出测试集，然后从训练集中划分出验证集
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.2, random_state=42)
X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.2, random_state=42)
def sigmoid(z):
    return 1 / (1 + np.exp(-z))
def sigmoid_derivative(z):
    return sigmoid(z) * (1 - sigmoid(z))
# 初始化参数
def initialize_parameters(input_size, hidden_size, output_size):
    np.random.seed(0)
    W1 = np.random.randn(input_size, hidden_size) * 0.01
    b1 = np.zeros((1, hidden_size))#第一层
    W3=np.random.randn(hidden_size,hidden_size)*0.01
    b3=np.zeros((1,hidden_size))#第二层
    W2 = np.random.randn(hidden_size, output_size) * 0.01
    b2 = np.zeros((1, output_size))#输出层
    print(W1.shape,b1.shape,W2.shape,b2.shape)
    print(b1.shape)
    return {'W1': W1, 'b1': b1,'W3':W3,'b3':b3, 'W2': W2, 'b2': b2}
def forward_propagation(X, parameters):
    W1, b1,W3,b3, W2, b2 = parameters['W1'], parameters['b1'],parameters['W3'],parameters['b3'], parameters['W2'], parameters['b2']

    # 隐藏层1
    Z1 = np.dot(X, W1) + b1
    A1 = sigmoid(Z1)

    #隐藏层2
    Z3=np.dot(A1,W3)+b3
    A3=sigmoid(Z3)

    # 输出层
    Z2 = np.dot(A3, W2) + b2
    A2 = sigmoid(Z2)

    cache = {'Z1': Z1, 'A1': A1,'Z3':Z3,'A3':A3, 'Z2': Z2, 'A2': A2}
    return A2, cache#A2是最后预测的输出
def compute_loss(A2, y, parameters, lambd=0.01):
    m = y.shape[0]  # 样本数
    W1, W2,W3 = parameters['W1'], parameters['W2'],parameters['W3']
    cross_entropy_loss = -np.sum(y * np.log(A2) + (1 - y) * np.log(1 - A2)) / m
    L2_regularization_cost = (lambd / (2 * m)) * (np.sum(np.square(W1)) + np.sum(np.square(W2)+np.sum(np.square(W3))))
    return cross_entropy_loss + L2_regularization_cost

def backward_propagation(parameters, cache, X, y, lambd=0.01):
    m = X.shape[0]
    W1, W2, W3 = parameters['W1'], parameters['W2'], parameters['W3']
    A1, A2, A3 = cache['A1'], cache['A2'], cache['A3']

    dZ2 = A2 - y
    dW2 = np.dot(A3.T, dZ2) / m + (lambd / m) * W2
    db2 = np.sum(dZ2, axis=0, keepdims=True) / m

    dZ3 = np.dot(dZ2, W2.T) * sigmoid_derivative(cache['Z3'])
    dW3 = np.dot(A1.T, dZ3) / m + (lambd / m) * W3
    db3 = np.sum(dZ3, axis=0, keepdims=True) / m

    dZ1 = np.dot(dZ3, W3.T) * sigmoid_derivative(cache['Z1'])
    dW1 = np.dot(X.T, dZ1) / m + (lambd / m) * W1
    db1 = np.sum(dZ1, axis=0, keepdims=True) / m

    grads = {'dW1': dW1, 'db1': db1, 'dW3': dW3, 'db3': db3, 'dW2': dW2, 'db2': db2}
    return grads


def update_parameters(parameters, grads, learning_rate=1.2):
    W1, b1, W3, b3, W2, b2 = parameters['W1'], parameters['b1'], parameters['W3'], parameters['b3'], parameters['W2'], parameters['b2']
    dW1, db1, dW3, db3, dW2, db2 = grads['dW1'], grads['db1'], grads['dW3'], grads['db3'], grads['dW2'], grads['db2']

    W1 -= learning_rate * dW1
    b1 -= learning_rate * db1
    W3 -= learning_rate * dW3
    b3 -= learning_rate * db3
    W2 -= learning_rate * dW2
    b2 -= learning_rate * db2

    return {'W1': W1, 'b1': b1, 'W3': W3, 'b3': b3, 'W2': W2, 'b2': b2}

def train_neural_network(X_train, y_train, X_val, y_val, hidden_size, num_iterations=10000, learning_rate=1.2):
    input_size = X_train.shape[1]
    output_size = 1

    parameters = initialize_parameters(input_size, hidden_size, output_size)

    train_losses = []
    val_losses = []

    for i in range(num_iterations):
        # 前向传播和计算损失
        A2_train, cache_train = forward_propagation(X_train, parameters)
        train_loss = compute_loss(A2_train, y_train,parameters)

        A2_val, _ = forward_propagation(X_val, parameters)
        val_loss = compute_loss(A2_val, y_val,parameters)

        # 反向传播
        grads = backward_propagation(parameters, cache_train, X_train, y_train)

        # 更新参数
        parameters = update_parameters(parameters, grads, learning_rate)

        # 保存训练过程中的损失值
        train_losses.append(train_loss)
        val_losses.append(val_loss)

        if i % 1000 == 0:
            print(f'第{i}次迭代后的训练集损失: {train_loss}, 验证集损失: {val_loss}')


    return parameters, train_losses, val_losses
def predict(parameters, X):
    A2, _ = forward_propagation(X, parameters)
    predictions = (A2 > 0.5).astype(int)
    return predictions
hidden_size = 30
parameters, train_losses, val_losses = train_neural_network(X_train, y_train, X_val, y_val, hidden_size,
                                                            num_iterations=10000, learning_rate=0.05)
# 在测试集上评估模型
predictions = predict(parameters, X_test)
accuracy = np.mean(predictions == y_test) * 100

print(f'测试集上的准确率: {accuracy}%')