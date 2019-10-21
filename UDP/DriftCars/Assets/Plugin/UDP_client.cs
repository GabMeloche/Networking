using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class UDP_client : MonoBehaviour
{
    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RunClient")]
    private static extern IntPtr RunClient();

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Send")]
    private static extern void Send(IntPtr p_client, float p_x, float p_y, float p_z);

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SendToUnity")]
    private static extern void SendToUnity(IntPtr p_client, ref float p_x, ref float p_y, ref float p_z);

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "DestroyClient")]
    private static extern void DestroyClient(IntPtr p_client);

    private IntPtr m_client;
    private Transform[] m_otherCars;
    public GameObject m_otherCar;
    // Start is called before the first frame update
    void Start()
    {
        m_client = RunClient();
        m_otherCar = GameObject.Find("CarRed(Clone)");
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log("real enemy pos: " + m_otherCar.transform.position);
        Vector3 pos = m_otherCar.transform.position;
        Send(m_client, pos.x, pos.y, pos.z);

        Vector3 posEnemy = Vector3.zero;
        SendToUnity(m_client, ref posEnemy.x, ref posEnemy.y, ref posEnemy.z);
        //posEnemy.x += 1;
        //m_otherCar.transform.position = posEnemy;
        Debug.Log("pos from dll: " + posEnemy);
    }

    void OnDestroy()
    {
        DestroyClient(m_client);
    }
}
